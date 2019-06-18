from django.db import models

# Create your models here.
class Matches(models.Model):
    match_id = models.IntegerField(unique=True)
    season = models.IntegerField()
    city = models.CharField(max_length=128)
    date = models.CharField(max_length=20)
    team1 = models.CharField(max_length=40)
    team2 = models.CharField(max_length=40)
    toss_winner = models.CharField(max_length=40)
    toss_decision = models.CharField(max_length=20)
    result = models.CharField(max_length=20)
    dl_applied = models.BooleanField()
    winner = models.CharField(max_length=40)
    win_by_runs = models.IntegerField()
    win_by_wickets = models.IntegerField()
    player_of_the_match = models.CharField(max_length=64)
    venue = models.CharField(max_length=128)
    umpire1 = models.CharField(max_length=64)
    umpire2 = models.CharField(max_length=64)
    umpire3 = models.CharField(max_length=64)


class Deliveries(models.Model):
    del_id = models.ForeignKey(Matches,on_delete=models.CASCADE,to_field="match_id")
    inning = models.IntegerField()
    batting_team = models.CharField(max_length=40)
    bowling_team = models.CharField(max_length=40)
    over = models.IntegerField()
    ball = models.IntegerField()
    batsman = models.CharField(max_length=64)
    non_striker = models.CharField(max_length=64)
    bowler = models.CharField(max_length=64)
    is_super_over = models.BooleanField()
    wide_runs = models.IntegerField()
    bye_runs = models.IntegerField()
    legbye_runs = models.IntegerField()
    noball_runs = models.IntegerField()
    penality_runs = models.IntegerField()
    batsman_runs = models.IntegerField()
    extra_runs = models.IntegerField()
    total_runs = models.IntegerField()
    player_dismissed = models.CharField(max_length=64)
    dismissal_kind = models.CharField(max_length=40)
    fielder = models.CharField(max_length=64)


