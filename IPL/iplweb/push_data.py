import os
import django
from openpyxl import load_workbook

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'IPL.settings')
django.setup()
from iplweb.models import *

def add_matches_data():
    fp = open('matches.csv','r')
    rows = len(fp.readlines())
    fp.close()
    fp = open('matches.csv','r')
    record = list(fp.readline()[:-1].split(','))
    for i in range(rows-1):
        record = list(fp.readline()[:-1].split(','))
        print(record)
        if(len(record)<15):
            continue
        c = Matches(match_id = int(record[0]),
                    season = int(record[1]),
                    city = record[2],
                    date = record[3],
                    team1 = record[4],
                    team2 = record[5],
                    toss_winner = record[6],
                    toss_decision = record[7],
                    result = record[8],
                    dl_applied = int(record[9]),
                    winner = record[10],
                    win_by_runs = int(record[11]),
                    win_by_wickets = int(record[12]),
                    player_of_the_match = record[13],
                    venue = record[14],
                    umpire1 = record[15],
                    umpire2 = record[16],
                    umpire3 = record[17])
        c.save()
    fp.close()

def add_deliveries_data():
    import MySQLdb
    conn = MySQLdb.connect('localhost', user='root', password='Fayaz#1233')
    cursor = conn.cursor()
    cursor.execute('use ipldb')
    fp = open('deliveries.csv','r')
    rows = len(fp.readlines())
    fp.close()
    fp = open('deliveries.csv', 'r')
    record = list(fp.readline()[:-1].split(','))
    for i in range(rows-1):
        record = list(fp.readline()[:-1].split(','))
        print(record)
        c = Deliveries(del_id_id = int(record[0]),inning = int(record[1]) ,batting_team = record[2],bowling_team = record[3],
                       over = int(record[4]),ball= int(record[5]),batsman = record[6],non_striker = record[7],
                       bowler = record[8],is_super_over = record[9],wide_runs = int(record[10]),bye_runs = int(record[11]),
                       legbye_runs = int(record[12]),noball_runs = int(record[13]),penality_runs = int(record[14]),
                       batsman_runs = int(record[15]),extra_runs = int(record[16]),total_runs = int(record[17]),
                       player_dismissed = record[18],dismissal_kind = record[19],fielder = record[20])
        c.save()
    fp.close()
#add_matches_data()
add_deliveries_data()
