from flask import Flask, render_template, request, redirect, url_for, session, flash
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


# home page

@app.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html')

# doctor pages

@app.route('/dashboard_doc', methods=['GET', 'POST'])
def dashboard_doc():
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

    return render_template('dashboard_doc.html', 
                           patient_table=[df_patient.to_html(classes='table table-striped table-sm', header=True, index=False, border=0, justify='left')],
                           appointment_table=[df_appointment.to_html(classes='table table-striped table-sm', header=True, index=False, border=0, justify='left')])

@app.route('/patients_doc', methods=['GET', 'POST'])
def patients_doc():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('patients_doc.html')

# admin pages

@app.route('/dashboard_admin', methods=['GET', 'POST'])
def dashboard_admin():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('dashboard_admin.html')


@app.route('/create_user', methods=['GET', 'POST'])
def create_user():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('create_user.html')

@app.route('/delete_user', methods=['GET', 'POST'])
def delete_user():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('delete_user.html')

# Front desk operator pages

@app.route('/dashboard_fdo', methods=['GET', 'POST'])
def dashboard_fdo():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('dashboard_fdo.html')


# Data entry operator pages

@app.route('/dashboard_deo', methods=['GET', 'POST'])
def dashboard_deo():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('dashboard_deo.html')

@app.route('/entry_deo', methods=['GET', 'POST'])
def entry_deo():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('entry_deo.html')

@app.route('/delete_deo', methods=['GET', 'POST'])
def delete_deo():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('delete_deo.html')

# login and logout

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
        if(username == "doctor" or username == "admin"):
            record = True
        if record:
            session['logged_in'] = True
        else:
            flash('Invalid credentials', category='error')
        if(username == "doctor"):
            return redirect('/dashboard_doc')
        elif(username == "admin"):
            return redirect('/dashboard_admin')
        
    return render_template('login.html')

@app.route('/logout')
def logout():
    session['logged_in'] = False
    return redirect('/')


# main function

def main():
    app.run(debug=True)

if __name__ == '__main__':
    main()