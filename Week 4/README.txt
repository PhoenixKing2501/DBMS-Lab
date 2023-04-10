To run the web application:
1. Ensure the following python libraries are installed
	Flask
	Flask-Login
	psycopg2
	secrets
	pandas
	bcrypt
	datetime
	getpass
	imsmtplib
	email
	argparse

2. Ensure postgresql is installed and running. Run the create_tables.sql and init_tables.sql to get the initial entries in the database.

3. In the line 28 of main.py, put the necessary username, password, database name, and host name for the postgresql database, by changing the code.

4. Run the main.py python file as:
	python main.py --email=<email> --password=<password>
	(Note: The password is the password for the email address. But currently, google doesn't allow to use the password for the email address. So, you can use the app password for the email address. To get the app password, go to https://myaccount.google.com/apppasswords and generate the app password. Enabling two step verification may be required.)

5. Open a web browser and navigate to http://localhost:10000

The web application will be running on the localhost port 10000. 