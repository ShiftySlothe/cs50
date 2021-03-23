-- Keep a log of any SQL queries you execute as you solve the mystery.
--What do the tables look like
.schema

--View crimescene report from the day
SELECT * FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2020;
--  Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. 
--  Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

--View interviews
SELECT * FROM interviews  WHERE day = 28 AND month = 7 AND year = 2020;

-- Phone call after the theft, lasts less than a minute, whoever they spoke to ordered a ticket.
-- Thief withdrew money before 10:15 
--Thief gets into car and drives away within 10 mins of theft

-- Get everyone who made a short phone call 
SELECT name FROM people INNER JOIN phone_calls ON people.phone_number = phone_calls.caller WHERE day = 25 AND month = 7 AND year = 2020 AND duration < 60;

--Roger
--Evelyn
--Ernest
--Evelyn
--Madison
--Russell
--Kimberly
--Bobby
--Victoria

--And recieved 
SELECT name FROM people INNER JOIN phone_calls ON people.phone_number = phone_calls.receiver WHERE day = 28 AND month = 7 AND year = 2020 AND duration < 60;
--Jack
--Larry
--Berthold
--Melissa
--James
--Philip
--Jacqueline
--Doris
--Anna

-- Get the relevant ATM transactions

SELECT name FROM people INNER JOIN bank_accounts ON people.id = bank_accounts.person_id INNER JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
   ...> WHERE atm_transactions.day = 28 AND atm_transactions.month = 7 AND atm_transactions.year = 2020 AND atm_transactions.atm_location = 'Fifer Street';
   
   
--Ernest
--Robert
--Russell
--Roy
--Bobby
--Elizabeth
--Danielle
--Madison
--Victoria

-- Get everyone who left

sqlite> SELECT name FROM people INNER JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
   ...> WHERE courthouse_security_logs.year = 2020
   ...> AND courthouse_security_logs.month = 7
   ...> AND courthouse_security_logs.day = 28
   ...> AND courthouse_security_logs.hour = 10
   ...> AND courthouse_security_logs.minute > 4
   ...> AND courthouse_security_logs.minute < 26;
   
-- Join it all together 
sqlite> SELECT name FROM people INNER JOIN courthouse_security_logs ON people.license_plate = courthouse_security_logs.license_plate
   ...> INNER JOIN bank_accounts ON people.id = bank_accounts.person_id INNER JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
   ...> INNER JOIN phone_calls ON people.phone_number = phone_calls.caller
   ...> WHERE phone_calls.day = 28
   ...> AND phone_calls.month = 7
   ...> AND phone_calls.year = 2020
   ...> AND atm_transactions.day = 28 AND atm_transactions.month = 7 AND atm_transactions.year = 2020 AND atm_transactions.atm_location = 'Fifer Street'
   ...> AND courthouse_security_logs.year = 2020
   ...>  AND courthouse_security_logs.month = 7
   ...> AND courthouse_security_logs.day = 28
   ...> AND courthouse_security_logs.hour = 10
   ...> AND courthouse_security_logs.minute > 4
   ...>  AND courthouse_security_logs.minute < 26;

-- Leaving Earnest or Russel 

-- Who did they speak to? 
SELECT name FROM people INNER JOIN phone_calls ON people.phone_number = phone_calls.receiver WHERE day = 28 AND month = 7 AND year = 2020 AND duration < 60 AND phone_calls.caller =
   ...> (SELECT phone_number FROM people WHERE people.name = 'Ernest' OR people.name = 'Russell');
   
--Both spoke to Philip

SELECT * FROM flights INNER JOIN passengers ON flights.id = passengers.flight_id
   ...> INNER JOIN people ON passengers.passport_number = people.passport_number
   ...> WHERE name = 'Russell';
   
SELECT * FROM flights INNER JOIN passengers ON flights.id = passengers.flight_id
   ...> INNER JOIN people ON passengers.passport_number = people.passport_number
   ...> WHERE name = 'Ernest';
   
-- But only one had a single flight! 

SELECT full_name FROM airports WHERE id = 4;

-- Which went to London