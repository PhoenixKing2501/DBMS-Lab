from flask import Flask, render_template, request, redirect, url_for, session
import psycopg2
import secrets


app = Flask(__name__)
app.secret_key = secrets.token_urlsafe(32)  # generate a random key for session


def get_db_conn():
    connection = psycopg2.connect(user="postgres",
                                password="432511",
                                host="localhost",
                                port="5432",
                                database="20CS30064",
                                )
    return connection

@app.route('/', methods=['GET', 'POST'])
def index():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('index.html')

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        print(username, password)
        # connection = get_db_conn()
        # cursor = connection.cursor()
        # cursor.execute("SELECT * FROM users WHERE username = %s AND password = %s", (username, password))
        # record = cursor.fetchone()
        # cursor.close()
        # connection.close()
        record = False
        if(username == "admin"):
            record = True
        if record:
            session['logged_in'] = True
        return redirect('/')
        
    return render_template('login.html')

def main():
    app.run(debug=True)

if __name__ == '__main__':
    main()