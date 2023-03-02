from flask import Flask, render_template, request, redirect, url_for
import psycopg2

app = Flask(__name__)


def get_db_conn():
    connection = psycopg2.connect(user="postgres",
                                password="432511",
                                host="localhost",
                                port="5432",
                                database="20CS30064",
                                )
    return connection

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/about')
def about():
    return "About Page"

def main():
    app.run(debug=True)

if __name__ == '__main__':
    main()