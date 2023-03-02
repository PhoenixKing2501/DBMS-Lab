from flask import Flask, render_template, request, redirect, url_for, session
import psycopg2
import secrets
import pandas as pd


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
    return render_template('index.html')

@app.route('/dashboard', methods=['GET', 'POST'])
def dashboard():
    if not session.get('logged_in'):
        return redirect('/login')
    
    connection=get_db_conn()
    cursor=connection.cursor()

    cursor.execute("SELECT * FROM patient")
    columns = [desc[0] for desc in cursor.description]
    records=cursor.fetchall()
    df_patient = pd.DataFrame(records, columns=columns)

    cursor.execute("SELECT * FROM appointment")
    columns = [desc[0] for desc in cursor.description]
    records=cursor.fetchall()
    df_appointment = pd.DataFrame(records, columns=columns)

    cursor.close()
    connection.close()

    return render_template('dashboard.html', 
                           patient_table=[df_patient.to_html(classes='table table-striped table-sm', header=True, index=False, border=0, justify='left')],
                           appointment_table=[df_appointment.to_html(classes='table table-striped table-sm', header=True, index=False, border=0, justify='left')])

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

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
        return redirect('/dashboard')
        
    return render_template('login.html')

@app.route('/logout')
def logout():
    session['logged_in'] = False
    return redirect('/')

def main():
    app.run(debug=True)

if __name__ == '__main__':
    main()