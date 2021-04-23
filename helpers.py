import os
import requests
import urllib.parse

from flask import redirect, render_template, request, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        url = f"https://cloud.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"

def GetCurrentCash(db):
    currentCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
    return currentCash

def GetPortfolioValue(db, ownedStocks, currentCash):
    totalPortfolioValue = 0
    totalPortfolioValue += currentCash
    for stock in ownedStocks:
        totalPortfolioValue += stock['currentValue']

    return totalPortfolioValue

def GetOwnedStocks(db):
    ##Get list of owned stocks
    ownedStocks = db.execute("SELECT ownedStocks.quantity, stocks.name, stocks.symbol, stocks.id FROM ownedStocks INNER JOIN stocks ON ownedStocks.stockID = stocks.id WHERE userID = ?", session["user_id"])
    ##Get current cash
    currentCash = GetCurrentCash(db)

    ##Add data to dictonary for each stock
    for stock in ownedStocks:
        if stock['quantity'] <= 0:
            ownedStocks.remove(stock)
            break
        stockPrice = lookup(stock['symbol'])['price']
        stockPriceStr = usd(stockPrice)
        stock['stockPrice'] = stockPrice
        stock['stockPriceStr'] = stockPriceStr
        stock['currentValue'] = stockPrice * stock['quantity']
        stock['currentValueStr'] = usd(stock['currentValue'])

    return ownedStocks

def GetStockHistory(db):
         stockHistory = db.execute("SELECT transactions.userID, stocks.symbol, transactions.price, transactions.isBuy, transactions.quantity, transactions.datetime FROM transactions INNER JOIN stocks ON transactions.stockID = stocks.id WHERE userID = ?", session["user_id"])
         for transaction in stockHistory:
             if transaction['isBuy'] == 1:
                 transaction['isBuy'] = True
             else:
                 transaction['isBuy'] = False
         
         return stockHistory
             