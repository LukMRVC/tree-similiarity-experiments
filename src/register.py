import sys
import argparse
import json
import psycopg2
from psycopg2 import sql

# http://initd.org/psycopg/docs/sql.html#module-psycopg2.sql
def store_tuple(table_name, values_dict):
    # Connect to database.
    db = psycopg2.connect("")
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

def store_algorithm(values_dict):
    store_tuple('algorithm', values_dict)

parser = argparse.ArgumentParser()
group = parser.add_mutually_exclusive_group(required=True)
group.add_argument(
    '-d', '--dataset',
    type=str,
    help="Path to dataset."
)
group.add_argument(
    '-a', '--algorithm',
    type=json.loads,
    help="Algorithm details as simple json file."
)
# parser.add_argument(
#     '--connection-string',
#     type=str,
#     dest='conn_string',
#     action='store',
#     help="Specify a custom database connection string (default: empty string)",
#     default="")
args = parser.parse_args()

if args.dataset is not None:
    example_dataset_values_dict = {
        'short_name' : 'D1',
        'filename' : 'dataset_1.bracket',
        'short_description' : 'First example dataset.',
        'number_trees' : 100,
        'avg_tree_size' : 10,
        'min_tree_size' : 10,
        'max_tree_size' : 10
    }
    store_dataset(example_dataset_values_dict)

if args.algorithm is not None:
    print(args.algorithm)
    store_algorithm(args.algorithm)
