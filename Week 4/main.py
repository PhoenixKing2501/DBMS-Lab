from flask import Flask, render_template, request, redirect, url_for, session, flash
import psycopg2
import secrets
import pandas as pd
import bcrypt


app = Flask(__name__)
app.secret_key = secrets.token_urlsafe(32)  # generate a random key for session


# utility functions

def get_db_conn():
    connection = psycopg2.connect(user="postgres",
                                password="432511",
                                host="localhost",
                                port="5432",
                                database="app_db",
                                )
    return connection

def get_hashed_password(password):
    return bcrypt.hashpw(password.encode('utf-8'), bcrypt.gensalt())

def check_password_hash(password, hash):
    return bcrypt.checkpw(password.encode('utf-8'), hash.encode('utf-8'))




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

    cursor.execute('''
    SELECT SSN AS "SSN", Name AS "Name", Phone AS "Phone"
    FROM patient
    WHERE PCP = %s;
    ''', (session['username'],))

    columns = [desc[0] for desc in cursor.description]
    records=cursor.fetchall()
    df_patient = pd.DataFrame(records, columns=columns)

    cursor.execute('''
    SELECT AppointmentID AS "Appointment ID", Patient AS "Patient", "Start", "End", ExaminationRoom AS "Examination Room"
    FROM appointment
    WHERE Physician = %s
        AND "Start" > CURRENT_TIMESTAMP;
    ''', (session['username'],))

    columns = [desc[0] for desc in cursor.description]
    records=cursor.fetchall()
    df_appointment = pd.DataFrame(records, columns=columns)

    cursor.close()
    connection.close()

    return render_template('dashboard_doc.html', 
                           patient_table=[df_patient.to_html(classes='table table-striped table-sm', header=True, index=False, border=0, justify='left')],
                           appointment_table=[df_appointment.to_html(classes='table table-striped table-sm', header=True, index=False, border=0, justify='left')])
# END dashboard_doc

@app.route('/patients_doc', methods=['GET', 'POST'])
def patients_doc():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('patients_doc.html')
# END patients_doc






# admin pages

@app.route('/dashboard_admin', methods=['GET', 'POST'])
def dashboard_admin():
    if not session.get('logged_in'):
        return redirect('/login')
    
    connection=get_db_conn()
    cursor=connection.cursor()

    cursor.execute('''
    SELECT EmployeeID AS "Employee ID", Name AS "Name", EmployeeType AS "User Type", SSN AS "SSN"
    FROM users;
    ''')

    columns = [desc[0] for desc in cursor.description]
    records=cursor.fetchall()
    df_users = pd.DataFrame(records, columns=columns)

    cursor.close()
    connection.close()

    return render_template('dashboard_admin.html',
                           user_table=[df_users.to_html(classes='table table-striped table-sm', header=True, index=False, border=0, justify='left')])


@app.route('/create_user', methods=['GET', 'POST'])
def create_user():
    if not session.get('logged_in'):
        return redirect('/login')
    
    if request.method == 'POST':
        # get form data
        name = request.form['name']
        employeeID = request.form['employeeID']
        employeeType = request.form['user']
        ssn = request.form['ssn']
        position = None
        departmentID = None
        if employeeType == 'doctor':
            position = request.form['position']
            departmentID = request.form['departmentID']
        email = request.form['email']
        address = request.form['address']
        country = request.form['country']
        state = request.form['state']
        pin = request.form['pin']

        password = get_hashed_password(employeeID)
        password = str(password)

        connection = get_db_conn()
        cursor = connection.cursor()

        address = address + ', ' + state + ', ' + country + ', ' + pin

        if email == '':
            email = None
        
        # insert into users
        cursor.execute('''
        INSERT INTO users
        VALUES (%s, %s, %s, %s, %s, %s, %s)
        ''', 
        (employeeID, name, employeeType, ssn, address, email, password)
        )

        if employeeType == 'doctor':

            # insert into doctor

            cursor.execute('''
            INSERT INTO doctor
            VALUES (%s, %s, %s)
            ''', 
            (employeeID, name, position)
            )

            # insert into affiliated_with

            cursor.execute('''
            INSERT INTO affiliated_with
            VALUES (%s, %s)
            ''',
            (employeeID, departmentID)
            )

        # save changes
        connection.commit()

        # close connection
        cursor.close()
        connection.close()
    # END if

    return render_template('create_user.html')
# END create_user

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

@app.route('/register_fdo', methods=['GET', 'POST'])
def register_fdo():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('register_fdo.html')

@app.route('/admit_fdo', methods=['GET', 'POST'])
def admit_fdo():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('admit_fdo.html')

@app.route('/discharge_fdo', methods=['GET', 'POST'])
def discharge_fdo():
    if not session.get('logged_in'):
        return redirect('/login')
    return render_template('discharge_fdo.html')









# Data entry operator pages

@app.route('/dashboard_deo', methods=['GET', 'POST'])
def dashboard_deo():
    if not session.get('logged_in'):
        return redirect('/login')
    
    connection=get_db_conn()
    cursor=connection.cursor()

    # upcoming appointments
    cursor.execute('''
    SELECT 
        apt.AppointmentID AS "Appointment ID", 
        pat.Name AS "Patient", 
        pat.SSN AS "Patient SSN", 
        doc.Name AS "Physician",
        doc.EmployeeID AS "Physician ID",
        apt."Start", 
        apt."End", 
        ExaminationRoom AS "Examination Room"
    FROM 
        appointment AS apt, patient AS pat, physician AS doc
    WHERE apt.Patient = pat.SSN
        AND apt.Physician = doc.EmployeeID
        AND apt."Start" > CURRENT_TIMESTAMP
    ORDER BY apt."Start";
    ''')

    columns = [desc[0] for desc in cursor.description]
    records=cursor.fetchall()
    df_apt = pd.DataFrame(records, columns=columns)

    cursor.close()
    connection.close()

    return render_template('dashboard_deo.html',
                           apt_table=[df_apt.to_html(classes='table table-striped table-sm', header=True, index=False, border=0, justify='left')])
# END dashboard_deo

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

        connection = get_db_conn()
        cursor = connection.cursor()
        # cursor.execute("SELECT * FROM users WHERE username = %s AND password = %s", (username, password))
        cursor.execute("SELECT EmployeeType, password FROM users WHERE EmployeeID = %s", (username,))
        record = cursor.fetchone()
        cursor.close()
        connection.close()

        if (record == None):
            flash('Invalid credentials', category='error')
            return redirect('/login')

        if (not check_password_hash(password, record[1][2:-1])):
            flash('Invalid credentials', category='error')
            return redirect('/login')

        session['logged_in'] = True
        session['username'] = username

        if(record[0] == "doctor"):
            return redirect('/dashboard_doc')
        elif(record[0] == "admin"):
            return redirect('/dashboard_admin')
        elif(record[0] == "front desk operator"):
            return redirect('/dashboard_fdo')
        elif(record[0] == "data entry operator"):
            return redirect('/dashboard_deo')
        
    return render_template('login.html')
# END login

@app.route('/logout')
def logout():
    session['logged_in'] = False
    return redirect('/')


# main function

def main():
    app.run(debug=True)

if __name__ == '__main__':
    main()