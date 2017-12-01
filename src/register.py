import sys
import argparse
import json
import psycopg2
from psycopg2 import sql

# http://initd.org/psycopg/docs/sql.html#module-psycopg2.sql
def store_tuple(table_name, values_dict):
    # Connect to database.
    db = psycopg2.connect("service=ted-join")
    # Open a cursor to perform database operations
    cur = db.cursor()
    attributes = values_dict.keys()
    query = sql.SQL("INSERT INTO {} ({}) VALUES ({})").format(
        sql.Identifier(table_name),
        sql.SQL(', ').join(map(sql.Identifier, attributes)),
        sql.SQL(', ').join(map(sql.Placeholder, attributes))
    )
    print(query.as_string(cur))
    cur.execute(query, values_dict)
    db.commit()
    # Close the cursor.
    cur.close()
    # Close communication with the database.
    db.close()

def store_dataset(values_dict):
    store_tuple('dataset', values_dict)

parser = argparse.ArgumentParser()
group = parser.add_mutually_exclusive_group(required=True)
group.add_argument(
    '-d', '--dataset',
    type=json.loads,
    help="Path to dataset."
)
args = parser.parse_args()

if args.dataset is not None:
    store_dataset(args.dataset)
