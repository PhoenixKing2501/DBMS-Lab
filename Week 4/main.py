from flask import Flask, render_template, request, redirect, url_for, session, flash
import psycopg2
import secrets
import pandas as pd
import bcrypt
import datetime


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
    SELECT AppointmentID AS "Appointment ID", Patient AS "Patient", "Start", "End"
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

    if request.method == 'POST':
        # get form data
        user = request.form['user']
        employeeID = user.split(' - ')[0]

        connection = get_db_conn()
        cursor = connection.cursor()

        # delete from users
        cursor.execute('''
        DELETE FROM users
        WHERE EmployeeID = %s
        ''', 
        (employeeID,)
        )

        # save changes
        connection.commit()

        # close connection
        cursor.close()
        connection.close()
    # END if

    connection = get_db_conn()
    cursor = connection.cursor()

    cursor.execute('''
    SELECT EmployeeID, Name, EmployeeType
    FROM users
    WHERE EmployeeID <> %s;
    ''', (session['username'],))

    records = cursor.fetchall()

    cursor.close()
    connection.close()

    user_list = []
    for record in records:
        user_list.append(record[0] + ' - ' + record[1] + ' - ' + record[2])

    return render_template('delete_user.html', user_list=user_list)
# END delete_user







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
    
    if request.method == 'POST':
        name = request.form['name']
        ssn = request.form['ssn']
        insuranceID = request.form['insuranceID']
        phone = request.form['phone']
        address = request.form['address']
        country = request.form['country']
        state = request.form['state']
        pin = request.form['pin']
        pcp = request.form['pcp']

        address = address + ', ' + state + ', ' + country + ', ' + pin

        connection = get_db_conn()
        cursor = connection.cursor()

        cursor.execute('''
        INSERT INTO patient
        VALUES (%s, %s, %s, %s, %s, %s)
        ''', (ssn, name, address, phone, insuranceID, pcp.split(' - ')[0]))

        connection.commit()
        cursor.close()
        connection.close()

        return redirect('/register_fdo')
    
    connection = get_db_conn()
    cursor = connection.cursor()

    # create a sql command for fetching list of Physican.id from Physician table
    cursor.execute('''SELECT EmployeeID, Name FROM physician;''')
    # fetch all the records from the database
    records = cursor.fetchall()
    # close the connection
    cursor.close()
    connection.close()

    pcp_list = []
    for record in records:
        pcp_list.append(record[0] + ' - ' + record[1])
    
    
    return render_template('register_fdo.html', pcp_list=pcp_list)

@app.route('/admit_fdo', methods=['GET', 'POST'])
def admit_fdo():
    if not session.get('logged_in'):
        return redirect('/login')
    
    if request.method == 'POST':
        room = request.form['room']
        room = room.split(' - ')[0]
        patient = request.form['patient']
        ssn = patient.split(' - ')[0]

        connection = get_db_conn()
        cursor = connection.cursor()

        cursor.execute('''SELECT COUNT(*) FROM stay;''')
        records = cursor.fetchall()
        stayID = 'STY' + str(records[0][0] + 3501)

        cursor.execute('''
        INSERT INTO stay
        VALUES (%s, %s, %s, CURRENT_TIMESTAMP, %s);
        ''', (stayID, ssn, room, None))

        cursor.execute('''UPDATE room SET Available = False WHERE Number = %s;''', (room,))

        connection.commit()
        cursor.close()
        connection.close()

        return redirect('/admit_fdo')
    # END if

    connection = get_db_conn()
    cursor = connection.cursor()

    # fetch rooms
    cursor.execute('''
    SELECT Number, Type FROM room
    WHERE Available = True;
    ''')

    records = cursor.fetchall()


    room_list = []
    for record in records:
        room_list.append(str(record[0]) + ' - ' + str(record[1]))

    # fetch patients
    cursor.execute('''SELECT SSN, Name FROM patient;''')

    records = cursor.fetchall()

    patient_list = []
    for record in records:
        patient_list.append(str(record[0]) + ' - ' + str(record[1]))

    cursor.close()
    connection.close()
    return render_template('admit_fdo.html', room_list=room_list, patient_list=patient_list)
# END admit_fdo

@app.route('/discharge_fdo', methods=['GET', 'POST'])
def discharge_fdo():
    if not session.get('logged_in'):
        return redirect('/login')
    
    if request.method == 'POST':
        patient = request.form['patient']
        ssn = patient.split(' - ')[0]

        connection = get_db_conn()
        cursor = connection.cursor()

        cursor.execute('''SELECT StayID, Room FROM stay WHERE Patient = %s AND "End" IS NULL;''', (ssn,))

        records = cursor.fetchall()
        
        stayID = records[0][0]
        room = records[0][1]

        cursor.execute('''
        UPDATE stay
        SET "End" = CURRENT_TIMESTAMP
        WHERE StayID = %s;
        ''', (stayID,))

        cursor.execute('''UPDATE room SET Available = True WHERE Number = %s;''', (room,))

        connection.commit()
        cursor.close()
        connection.close()

        return redirect('/discharge_fdo')
    # END if

    connection = get_db_conn()
    cursor = connection.cursor()

    # fetch patients
    cursor.execute('''
    SELECT SSN, Name 
    FROM patient, stay
    WHERE patient.SSN = stay.Patient
        AND stay."End" IS NULL;
    ''')

    records = cursor.fetchall()

    patient_list = []
    for record in records:
        patient_list.append(str(record[0]) + ' - ' + str(record[1]))

    return render_template('discharge_fdo.html', patient_list=patient_list)
# END discharge_fdo





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
        apt."End"
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
    
    if request.method == 'POST':
        # get form data
        patient = request.form['patient']
        ssn = patient.split(' - ')[0]
        entryType = request.form['entryType']

        # connect to database

        connection = get_db_conn()
        cursor = connection.cursor()

        if entryType == 'medication':
            physician = request.form['medphysician']
            physician = physician.split(' - ')[0]
            medication = request.form['medication']
            medication = medication.split(' - ')[0]
            dose = request.form['dose']
            appointmentID = request.form['appointmentID']
            if appointmentID == '':
                appointmentID = None

            # insert into prescribes
            cursor.execute('''
            INSERT INTO prescribes
            VALUES (%s, %s, %s, CURRENT_TIMESTAMP, %s, %s);
            ''', 
            (physician, ssn, medication, appointmentID, dose)
            )


        elif entryType == 'treatment':
            physician = request.form['testphysician']
            physician = physician.split(' - ')[0]
            procedure = request.form['procedure']
            procedure = procedure.split(' - ')[0]
            date = request.form['date']

            # get stayID 
            cursor.execute('''SELECT StayID FROM stay WHERE Patient = %s AND "End" IS NULL;''', (ssn,))
            # insert into treatment
            record = cursor.fetchone()

            if record == None: # no stay record
                flash('Patient not admitted',category='error')
                return redirect('/entry_deo')
            stayID = record[0]
            cursor.execute('''
            INSERT INTO treatment
            VALUES (%s, %s, %s, %s ,%s);
            ''', 
            (ssn, procedure, stayID, date, physician)
            )


        elif entryType == 'appointment':
            physician = request.form['appphysician']
            physician = physician.split(' - ')[0]
            date = request.form['date']
            time = request.form['time']
            # examinationRoom = request.form['examinationRoom']
            # insert into appointment
            start = date + ' ' + time.split('-')[0]
            end = date + ' ' + time.split('-')[1]

            
            # check if appointment is available
            cursor.execute('''SELECT COUNT(*) FROM appointment WHERE "Start" < %s AND "End" > %s;''', (end, start))
            record = cursor.fetchone()
            if record[0] > 0 or start < datetime.now().strftime('%Y-%m-%d %H:%M:%S'):
                flash('Appointment not available',category='error')

                
            


            cursor.execute('''
            INSERT INTO appointment
            VALUES (%s, %s, %s, %s, %s);
            ''', 
            (appointmentID, ssn, physician, start, end)
            )

        # save changes
        connection.commit()

        # close connection
        cursor.close()
        connection.close()

    # get the list of ssn patient from database
    connection = get_db_conn()
    cursor = connection.cursor()
    
    cursor.execute('''SELECT SSN, Name FROM patient;''')
    # store in records 
    records = cursor.fetchall()
    patient_list = []
    for patient in records:
        patient_list.append(str(patient[0]) + ' - ' + str(patient[1]))
        
    # get the list of physicians from database
    cursor.execute('''SELECT EmployeeID, Name FROM physician;''')
    # store in records 
    records = cursor.fetchall()
    physician_list = []
    for physician in records:
        physician_list.append(str(physician[0]) + ' - ' + str(physician[1]))
        
    # get the list of medications from database
    cursor.execute('''SELECT Code,Name FROM medication;''')
    # store in records
    records = cursor.fetchall()
    medication_list = []
    for medication in records:
        medication_list.append(str(medication[0]) + ' - ' + str(medication[1]))
        
    # get the list of procedures from database
    cursor.execute('''SELECT Code,Name FROM procedure;''')
    # store in records
    records = cursor.fetchall()
    procedure_list = []
    for procedure in records:
       procedure_list.append(str(procedure[0]) + ' - ' + str(procedure[1])) 
    
    return render_template('entry_deo.html',patient_list=patient_list, procedure_list=procedure_list,
                           medication_list=medication_list,physician_list=physician_list)

@app.route('/delete_deo', methods=['GET', 'POST'])
def delete_deo():
    if not session.get('logged_in'):
        return redirect('/login')
    
    if request.method == 'POST':
        user = request.form['user']
        # delete user from database table users
        connection = get_db_conn()
        
        
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
        else:
            return redirect('/login')
        
    return render_template('login.html')
# END login

@app.route('/logout')
def logout():
    session['logged_in'] = False
    return redirect('/')




# profile for all users
@app.route('/profile', methods=['GET', 'POST'])
def profile():
    if not session.get('logged_in'):
        return redirect('/login')
    
    if request.method == 'POST':
        if request.form['ifchange'] == 'on':
            # change password
            old = request.form['old']
            new = request.form['new']
            confirm = request.form['confirm']

            if new != confirm:
                flash('Passwords do not match', category='error')
                return redirect('/profile')
            
            connection = get_db_conn()
            cursor = connection.cursor()
            cursor.execute("SELECT password FROM users WHERE EmployeeID = %s", (session['username'],))
            record = cursor.fetchone()
            cursor.close()
            connection.close()

            if (not check_password_hash(old, record[0][2:-1])):
                flash('Invalid password', category='error')
                return redirect('/profile')
            
            connection = get_db_conn()
            cursor = connection.cursor()

            cursor.execute("UPDATE users SET password = %s WHERE EmployeeID = %s", (get_hashed_password(new), session['username']))

            connection.commit()
            cursor.close()
            connection.close()

            return redirect('/profile')
        # END if
    # END if


    connection = get_db_conn()
    cursor = connection.cursor()

    cursor.execute("SELECT * FROM users WHERE EmployeeID = %s;", (session['username'],))

    record = cursor.fetchone()
    cursor.close()
    connection.close()

    return render_template('profile.html', name=record[1], employeeid=record[0], ssn=record[3], userType=record[2], email=record[5], address=record[4])
# END profile

# main function

def main():
    app.run(debug=True,port = 10000)

if __name__ == '__main__':
    main()