import datetime
import os
import re

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
    
    # Get symbol and quantity
    shares_list = db.execute("SELECT symbol, quantity FROM users_shares\
         WHERE u_id = :u_id", u_id=session["user_id"])
    
    # Get user balance and balance + value of shares
    balance = round(db.execute("SELECT cash FROM users WHERE id = :u_id", u_id=session["user_id"])[0]['cash'], 2)
    grand_total = balance
    
    # Calculate total value and get current price
    for share in shares_list:
        api_response = lookup(share['symbol'])
        total_value = share['quantity'] * api_response['price']
        
        grand_total += total_value
        
        # Add total_value and current_price to share
        share['current_price'] = api_response['price']
        share['total_value'] = round(total_value, 2)
    grand_total = round(grand_total, 2)
    
    return render_template("index.html", shares=shares_list, balance=balance, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST 
    if request.method == 'POST':
        symbol = request.form.get("symbol")
        api_response = lookup(symbol)

        # Ensure symbol exists
        if not api_response:
            flash("Provided symbol doesn't exist!")
            return redirect('/buy')

        # Get the quantity, must be a number
        try:
            quantity = int(request.form.get("shares"))
        except ValueError:
            flash("You must provide an integer.")
            return redirect('/buy')

        # Ensure quantity is a positive number
        if quantity <= 0:
            flash("You must provide a positive number different than zero.")
            return redirect('/buy')

        # Get user cash
        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        user_cash = rows[0]["cash"]
        
        # Get price
        price = round((api_response["price"] * quantity), 2)

        # Ensure user has enough cash
        if user_cash < price:
            flash("You don't have enough money.")
            return redirect('/buy')

        balance = user_cash - price

        # Update users table in database
        symbol = api_response['symbol']
        db.execute("UPDATE users SET cash = :balance WHERE id = :user_id", balance=balance, user_id=session["user_id"])
        
        # Get transaciton date
        date = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")

        # Insert new transaction into database
        db.execute("INSERT INTO transactions (u_id, transaction_date, price_whole, symbol, quantity, type)\
         VALUES(:u_id, :date, :price, :symbol, :quantity, :type)",\
              u_id=session["user_id"], date=date, price=price, symbol=symbol, quantity=quantity, type="buy")
        
        # Check if user already has shares with that symbol
        shares = db.execute("SELECT * FROM users_shares WHERE symbol = :symbol AND u_id = :u_id",\
             symbol=symbol, u_id=session["user_id"])
        if len(shares) != 1:
            # If not insert 
            db.execute("INSERT INTO users_shares (u_id, symbol, quantity) VALUES(:u_id, :symbol, :quantity)",\
                 u_id=session["user_id"], symbol=symbol, quantity=quantity)
        else:
            # Else update
            new_quantity = shares[0]['quantity'] + quantity
            db.execute("UPDATE users_shares SET quantity = :new_quantity WHERE symbol = :symbol AND u_id = :u_id",\
                 new_quantity=new_quantity, symbol=symbol, u_id=session["user_id"])
        
        flash(f"Bought {symbol} - {quantity} shares for {price}$!")
        return redirect('/buy')

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get user transaction
    transaction_rows = db.execute("SELECT transaction_date, price_whole, symbol, quantity, type FROM transactions WHERE u_id = :u_id ORDER BY transaction_date",\
            u_id=session['user_id'])
    return render_template("history.html", transaction_rows=transaction_rows)


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
    
    # User reached route via POST 
    if request.method == 'POST':
        symbol = request.form.get("symbol")
        api_response = lookup(symbol)
        
        # Ensure symbol exists
        if not api_response:
            return apology("Provided symbol doesn't exist!", 403)

        return render_template("quoted.html", symbol=symbol, api_response=api_response)
    
    # User reached route via GET
    return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    # https://stackoverflow.com/a/43111707
    r_p = re.compile('^(?=\\S{6,20}$)(?=.*?\\d)(?=.*?[a-z])(?=.*?[A-Z])(?=.*?[^A-Za-z\\s0-9])')

    # If current session redirect to '/'
    if session.get("user_id"):
        return redirect("/")
    
    # If new user
    if request.method == "POST":
        
        user_name = request.form.get("username")
        password = request.form.get("password")
        
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=user_name)

        # Ensure username was submitted
        if not user_name:
            flash(f"You must provide an username!")
            return redirect('/register')
        
        # Check if such user exists in database:
        elif rows:
            flash(f"User already exists.")
            return redirect('/register')

        # Ensure password was submitted
        elif not password:
            flash(f"You must provide a password")
            return redirect('/register')

        # Ensure password was submitted again
        elif not request.form.get("confirmation"):
            flash(f"You must provide a password again.")
            return redirect('/register')
        
        elif re.search(r_p, password) is None:
            flash("Password must be of length 6-20, contain at least one digit,\
                     at least one uppercase and lowercase letter and at least one special character.")
            return redirect('/register')

        # passwords match
        elif password != request.form.get("confirmation"):
            flash(f"Passwords do not match!")
            return redirect('/register')


        # Hash password
        password_hash = generate_password_hash(request.form.get("password"))
        
        # Save user to database
        db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=user_name, hash=password_hash)

        # Redirect user to login page
        return redirect("/login")
    
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST
    if request.method == "POST":
        symbol = request.form.get("symbol")
        
        # Ensure user selected symbol
        if not symbol:
            flash(f"You must select a share to sell!")
            return redirect('/sell')
        
        try:
            num_of_shares = int(request.form.get("num_of_shares"))
        except ValueError:
            flash(f"You must provide an integer.")
            return redirect('/sell')
            

        # Ensure user provided number of shares to sell
        if not num_of_shares:
            flash(f"You must provide number of shares to sell!")
            return redirect('/sell')
        
        selected_share = db.execute("SELECT quantity FROM users_shares WHERE u_id = :u_id AND symbol = :symbol",\
                u_id=session["user_id"], symbol=symbol)[0]
        
        # Ensure number is in range of owned number of shares
        if num_of_shares < 1 or num_of_shares > selected_share['quantity']:
            flash(f"Wrong number of sharess!")
            return redirect('/sell')

        # When number of shares to sell is equal to number of shares owned -> delete from database
        if num_of_shares == selected_share['quantity']:
            db.execute("DELETE FROM users_shares WHERE u_id = :u_id AND symbol = :symbol",\
                u_id=session["user_id"], symbol=symbol)
        else:
            new_quantity = selected_share['quantity'] - num_of_shares
            db.execute("UPDATE users_shares SET quantity = :new_quantity WHERE u_id = :u_id AND symbol = :symbol",\
                new_quantity=new_quantity, u_id=session["user_id"], symbol=symbol)

        # Get transaciton date
        date = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")

        # Add cash to user's account
        api_response = lookup(symbol)
        price = round(api_response['price'] * num_of_shares, 2)
        db.execute("UPDATE users SET cash = cash + :add_to_balance WHERE id = :u_id",\
                add_to_balance=price, u_id=session["user_id"])

        # Add to transactions
        db.execute("INSERT INTO transactions (u_id, transaction_date, price_whole, symbol, quantity, type)\
         VALUES(:u_id, :date, :price, :symbol, :quantity, :type)",\
              u_id=session["user_id"], date=date, price=price, symbol=symbol, quantity=num_of_shares, type="sell")

        flash(f"Sold {symbol} - {num_of_shares} for {price}$")
        return redirect('/sell')

    # User reached route via GET
    shares_list = db.execute("SELECT symbol, quantity FROM users_shares\
         WHERE u_id = :u_id", u_id=session["user_id"])
    
    return render_template("sell.html", shares_list=shares_list)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
