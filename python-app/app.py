from flask import Flask, render_template, Response, make_response, request, redirect, url_for
import random

app = Flask(__name__)



user_dict = {}

def is_authenticated(request):
   userID = request.cookies.get('userID')
   if (not userID):
      return False
   
   name = user_dict.get(userID)
   if (not name):
      return False

   return True


@app.route('/')
def index():
   if (is_authenticated(request)):
      return make_response(redirect(url_for('dashboard')))

   return make_response(render_template('index.html'))


@app.route('/login', methods = ['POST'])
def login():
   if request.method == 'POST':
      user = request.form['user']
      password = request.form['pass']

      if (user == "admin" and password == "admin123"):
         userID = str(random.getrandbits(128))
         user_dict[userID] = user

         resp = make_response(redirect(url_for('dashboard')))

         # Secure cookie flag not set
         resp.set_cookie('userID', userID)
         return resp
      else:
         return make_response(render_template('index.html', error='Invalid credentials'))


@app.route('/logout')
def logout():
   # UserID not removed on logout
   resp = make_response(redirect(url_for('index')))
   resp.delete_cookie('userID')
   return resp


@app.route('/dashboard')
def dashboard():
   if (not is_authenticated(request)):
      return "User not authenticated", 401

   userID = request.cookies.get('userID')
   name = user_dict.get(userID)

   return make_response(render_template('dashboard.html', name=name))
