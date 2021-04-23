import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from helpers import apology, login_required, lookup, usd, GetCurrentCash, GetPortfolioValue, GetOwnedStocks, GetStockHistory

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    currentCash = GetCurrentCash(db)
    ownedStocks = GetOwnedStocks(db)
    totalPortfolioValue = GetPortfolioValue(db, ownedStocks, currentCash)

    return render_template("index.html", ownedStocks=ownedStocks, currentCash=currentCash, totalPortfolioValue=totalPortfolioValue)

@app.route("/bank", methods=["GET", "POST"])
@login_required
def bank():
    """Add cash"""
    if request.method == "POST":
        #Validate form
        cashToAdd = float(request.form.get("cash"))
        currentCash = GetCurrentCash(db)
        newCash = currentCash + cashToAdd
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, session["user_id"])

        return redirect("/bank")
    else:
        currentCash = GetCurrentCash(db)
        ownedStocks = GetOwnedStocks(db)
        totalPortfolioValue = GetPortfolioValue(db, ownedStocks, currentCash)
        return render_template("bank.html", ownedStocks=ownedStocks, currentCash=currentCash, totalPortfolioValue=totalPortfolioValue)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Get stock quote."""
    if request.method == "POST":
        #Parse and validate form data
        stockSymbol = request.form.get("symbol")

        if not stockSymbol:
            return apology("must provide symbol", 400)

        buyQuantity = request.form.get("shares")
        if not buyQuantity:
            return apology("must provide quantity", 400)
    
        try:
            buyQuantity = int(buyQuantity)
            if buyQuantity < 1:  # if not a positive int print message and ask for input again
                return apology("must provide positive integer", 400)
        except ValueError:
            return apology("must provide an integer", 400)  
            

        #Get stock data via API
        stockData = lookup(stockSymbol)
        if not stockData:
            return apology("failed to find stock data, please check symbol", 400)

        #Check for stock in DB, add if not found
        stock = db.execute("SELECT * FROM stocks WHERE symbol = ?", stockData['symbol'])
        if(len(stock) != 1):
            db.execute("INSERT INTO stocks (name, lastPrice, symbol) VALUES (?, ?, ?)", stockData['name'], stockData['price'], stockData['symbol']);


        stockID = db.execute("SELECT id FROM stocks WHERE symbol = ?", stockData['symbol'])
        stockID= stockID[0]['id']

        #Query whether user can afford stock
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        userCash = userCash[0]['cash']

        transactionTotalCost = float(stockData['price']) * buyQuantity
        if(transactionTotalCost > userCash):
            return apology("you're skint, try again later", 403)
        #Buy stock
        else:
            #Update User Cash
            updatedUserCash = userCash - transactionTotalCost
            db.execute("UPDATE users SET cash = ? WHERE id = ?", updatedUserCash, session["user_id"])
            #Update Tranactions
            timestamp = datetime.now()
            timestampStr = timestamp.strftime("%m/%d/%Y, %H:%M:%S")
            db.execute("INSERT INTO transactions (userID, stockID, isBuy, price, quantity, datetime) VALUES (?, ?, 1, ?, ?, ?)", session["user_id"], stockID, stockData['price'], buyQuantity, timestampStr)
            #Update Inventory
            userInventory = db.execute("SELECT * FROM ownedStocks WHERE userID = ? AND stockID = ?", session["user_id"], stockID)
            if(len(userInventory) != 1):
                db.execute("INSERT INTO ownedStocks (userID, stockID, quantity) VALUES (?, ?, ?)", session["user_id"], stockID, buyQuantity)
            else:
                userStockQuantity = userInventory[0]['quantity'] + buyQuantity
                db.execute("UPDATE ownedStocks SET quantity = ? WHERE userID = ? AND stockID = ?", userStockQuantity, session["user_id"], stockID)

        return redirect("/buy")
    ##Method GET
    else:
        currentCash = GetCurrentCash(db)
        currentCashStr = usd(currentCash)
        ownedStocks = GetOwnedStocks(db)
        totalPortfolioValue = usd(GetPortfolioValue(db, ownedStocks, currentCash))

        return render_template("buy.html", ownedStocks=ownedStocks, currentCash=currentCashStr, totalPortfolioValue=totalPortfolioValue)


@app.route("/history")
@login_required
def history():
        stockHistory = GetStockHistory(db)
        currentCash = GetCurrentCash(db)
        ownedStocks = GetOwnedStocks(db)
        totalPortfolioValue = GetPortfolioValue(db, ownedStocks, currentCash)
        return render_template("history.html", ownedStocks=ownedStocks, currentCash=currentCash, totalPortfolioValue=totalPortfolioValue, stockHistory=stockHistory)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        stockSymbol = request.form.get("symbol")

        if not stockSymbol:
            return apology("must provide symbol", 400)

        #Get stock data via API
        stockData = lookup(stockSymbol)
        stockData['price'] = usd(stockData['price'])
        if not stockData:
            return apology("failed to find stock data, please check symbol", 400)

        return render_template("quoted.html", stockData = stockData)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        password = request.form.get("password")
        passwordCheck = request.form.get("confirmation")

        if password != passwordCheck:
            return apology("passwords must match", 400)

        # Query database for existing username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        if len(rows) == 1:
            return apology("username taken", 400)

        #Add user to DB
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("registration failure, please resubmit", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        ##Validate form
        if not request.form.get("stock") or not request.form.get("sellQuantity"):
            return apology('form error, please resubmit', 403)

        stockSymblToSell = request.form.get("stock")
        sellQuantity = float(request.form.get("sellQuantity"))

        ##Check user owns stock, grab price
        stockOwned = False
        stockData = {}

        ownedStocks = GetOwnedStocks(db)
        for stock in ownedStocks:
            if stock['symbol'] == stockSymblToSell:
                stockOwned = True
                stockData = stock
                break
        app.logger.info(stockData)
        if stockOwned:
            ##Check enough stock is owned
            quantityOwned = db.execute("SELECT ownedStocks.quantity, stocks.symbol FROM ownedStocks INNER JOIN stocks ON ownedStocks.stockID = stocks.id WHERE userID = ? AND stocks.symbol = ?", session["user_id"], stockSymblToSell)
            if len(quantityOwned) != 1:
                return apology('db error', 403)
            quantityOwned = quantityOwned[0]['quantity']

            if sellQuantity > quantityOwned:
                return apology('need more stonks', 403)
            ##Process sale
            else:
                #Update Quantity
                stocksRemaining = quantityOwned - sellQuantity
                db.execute("UPDATE ownedStocks SET quantity = ? WHERE userID = ? AND stockID = (SELECT id FROM stocks WHERE symbol = ?)", stocksRemaining, session["user_id"], stockSymblToSell)

                #Update cash
                sellPrice = sellQuantity * stockData['stockPrice']
                newCash = GetCurrentCash(db) + sellPrice
                db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, session["user_id"])

                #Update transactions
                timestamp = datetime.now()
                timestampStr = timestamp.strftime("%m/%d/%Y, %H:%M:%S")
                db.execute("INSERT INTO transactions (userID, stockID, isBuy, price, quantity, datetime) VALUES (?, ?, 0, ?, ?, ?)", session["user_id"], stockData['id'], stockData['stockPrice'], sellQuantity, timestampStr)

        else:
            return apology('need more stonks', 403)

        return redirect("/sell")
    else:
        currentCash = GetCurrentCash(db)
        ownedStocks = GetOwnedStocks(db)
        totalPortfolioValue = GetPortfolioValue(db, ownedStocks, currentCash)

        return render_template("sell.html", ownedStocks=ownedStocks, currentCash=currentCash, totalPortfolioValue=totalPortfolioValue)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
