import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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
    """Show portfolio of stocks"""

    # SQL query: "SELECT SUM(shares), stock FROM history WHERE user_id = 6 GROUP BY stock;
    holdings = db.execute("SELECT SUM (shares), stock FROM history WHERE user_id = :user GROUP BY stock HAVING shares > 0", user=session["user_id"])

    # add necessary info to the SQL pull in order to round out the information (missing name) AND pull the most recent price.
    for holding in holdings:
        stock = lookup(holding["stock"])
        holding["price"] = stock["price"]
        holding["name"] = stock["name"]

    # add up the current total value of all the holdings
    total_holdings = 0

    for holding in holdings:
        holding_sum = holding["SUM (shares)"] * holding["price"]

        total_holdings = total_holdings + holding_sum


    return render_template("index.html", holdings=holdings, total_holdings=total_holdings)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # GET return
    if request.method == "GET":
        return render_template("buy.html")

    # POST return
    elif request.method == "POST":

        # pull string from form and ensure it is uppercase
        extractstring = request.form.get("symbol")
        symbol = extractstring.upper()

        # extract number of shares
        shares = request.form.get("shares")

        # pull info from API
        stock = lookup(symbol)

        # assign the price
        price = stock["price"]

        # make sure no fields are left empty
        if not symbol:
            return apology("enter a stock")
        if not shares:
            return apology("enter number of shares")

        #check to be sure it is not a char
        if shares.isdigit() is False:
            return apology("enter only digits")

        # make sure number of shares is positive
        if int(shares) <= 0:
            return apology("insert positive number")

        # make sure stock exists
        if stock == None:
            return apology("not a valid stock")

        # search to see how much cash the user has
        cash_holdings = db.execute("SELECT cash FROM users WHERE id = :user", user = session["user_id"])

        # break value of cash from dict.
        cash = cash_holdings[0]['cash']

        # calculate total cost of purchase
        total_cost = float(shares) * float(price)

        # check to see if cash is sufficient for purchase
        if float(cash) < total_cost:
            return apology("you broke homie")

        # if the user has enough cash
        elif float(cash) >= total_cost:

            # update the amount of cash the user has
            db.execute("UPDATE users SET cash = (cash - :total_cost) WHERE id= :user", total_cost = total_cost, user = session["user_id"])

            newcashamount = db.execute("SELECT cash FROM users WHERE id = :user", user = session["user_id"])

            # log the buy into the history table
            db.execute("INSERT INTO history (user_id, BuySell, stock, price, shares, total) VALUES (:user, 'buy', :stock, :price, :shares, :total)", user = session["user_id"], stock = symbol, price = float(price), shares = shares, total = total_cost)

            return redirect("/")

@app.route("/donate", methods=["GET", "POST"])
@login_required
def donate():
    """Buy shares of stock"""
    # GET return
    if request.method == "GET":
        return render_template("donate.html")

    # POST return
    elif request.method == "POST":

        #declare cash amount
        donation = request.form.get("donation")

        #check to be sure it is not a char
        if donation.isdigit() is False:
            return apology("enter only digits.")

        #check to be sure it is positive
        if int(donation) <= 0:
            return apology("insert positive number")

        # update the amount of cash the user has
        db.execute("UPDATE users SET cash = (cash - :donation) WHERE id= :user", donation = donation, user = session["user_id"])

        # update charity fund
        db.execute("UPDATE users SET cash = (cash + :donation) WHERE username = 'charity'", donation = donation)

        return redirect("/")









@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # run an SQL query to extract all transactions of user
    transactions = db.execute("SELECT * FROM history WHERE user_id = :user", user=session["user_id"])

    return render_template("history.html", transactions = transactions)




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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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

    # return the regular webpage if request is GET
    if request.method == "GET":
        return render_template("quote.html")

    # if the person is selecting a stock
    elif request.method == "POST":

        # declare symbol
        symbol = request.form.get("symbol")

        # check to be sure something is inserted
        if not symbol:
            return apology("insert a stock")

        # lookup stock
        stock = lookup(symbol)

        if stock == None:
            return apology("not a valid symbol")

        # return the quoted page, passing in the stock value
        return render_template("quoted.html", stock = stock)



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # requests via GET
    if request.method == "GET":
        return render_template("register.html")


    # otherwise the request is via POST
    else:
        # declare some variables
        username = request.form.get("username")
        password = request.form.get("password")
        confirm_password = request.form.get("confirm_password")

        # do some checking to be sure things aren't incomplete
        if request.form.get("password") != request.form.get("confirm_password"):
            return apology("Passwords don't match!")
        elif username == "":
            return apology("Please enter a username")
        elif password == "":
            return apology("Please enter a password")
        elif confirm_password == "":
            return apology("Please confirm the password")

        # if everything checks out
        else:
            # hash the password
            hash_password = generate_password_hash(password)

            # check to see if username is taken
            username_check = db.execute("SELECT * FROM users WHERE username = :username", username = username)
            if len(username_check) > 0:
                return apology("pick another username")

            # insert into database
            insert = db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)", username = username, password = hash_password)

            #pull out user to create session
            rows = db.execute("SELECT * FROM users WHERE username = :username", username = username)

            # Remember which user has logged in. Copied from /login
            session["user_id"] = rows[0]["id"]

            # Redirect user to home page
            return redirect("/")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # run an SQL query to get users holdings
    holdings = db.execute("SELECT stock, SUM(shares) FROM history WHERE user_id = :user GROUP BY stock HAVING shares > 0", user = session["user_id"])

    #if the request method is get, send the holdings for the select menu.
    if request.method == "GET":

        return render_template("sell.html", holdings = holdings)

    elif request.method == "POST":

         # pull string from form and ensure it is uppercase
        extractstring = request.form.get("symbol")
        symbol = extractstring.upper()

        # extract number of shares
        shares = request.form.get("shares")

        # pull info from API
        stock = lookup(symbol)

        # assign the price
        price = stock["price"]

        # make sure no fields are left empty
        if not shares:
            return apology("enter number of shares")

        #check to be sure it is not a char
        if shares.isdigit() is False:
            return apology("this is not a digit")

        # make sure number of shares is positive
        if int(shares) <= 0:
            return apology("insert positive number")

        # ADD IN CHECK FOR FLOATS AND CHARS

        # run an SQL query to be sure the user has enough of the stock he wants to sell
        checkshares = db.execute("SELECT stock, SUM(shares) FROM history WHERE user_id = :user AND stock = :stock", user = session["user_id"], stock = symbol)
        # select proper item from list and convert it to an int
        totalshares = int(checkshares[0]["SUM(shares)"])
        # make the comparison
        if totalshares < int(shares):
            return apology("not enough shares :(")

        #declare the total value of the transaction
        total_value = float(shares) * -float(price)

        # declare a negative shares value to insert into the SQL query
        negative_shares = -int(shares)

        # log the sell in the history table
        db.execute("INSERT INTO history (user_id, BuySell, stock, price, shares, total) VALUES (:user, 'sell', :stock, :price, :shares, :total)", user = session["user_id"], stock = symbol, price = float(price), shares = negative_shares, total = total_value)

        # update cash holdings in the users database. Minus because the total_value is negative.
        db.execute("UPDATE users SET cash = (cash - :total_value) WHERE id= :user", total_value = total_value, user = session["user_id"])

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
