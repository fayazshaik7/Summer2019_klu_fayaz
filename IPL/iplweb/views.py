from django.contrib.auth.models import User
from django.shortcuts import render, redirect
from django.views import View
from iplweb.models import *
# Create your views here.
from django import forms
from django.contrib.auth import authenticate, login, logout
from django.db.models import Count
from django.contrib.auth.mixins import LoginRequiredMixin
from django.db.models import Q,Sum
from django.core.paginator import Paginator
class Matches_view(View):
    def get(self,request,**kwargs):
        if kwargs:
            c = Matches.objects.filter(season = kwargs['year'])
        else:
            c = Matches.objects.filter(season = 2019)
            kwargs['year'] = 2019
        paginator = Paginator(c, 8)
        page = request.GET.get('page')
        c = paginator.get_page(page)
        res = dict()
        for i in c:
            c6 = Deliveries.objects.filter(del_id_id=i.match_id, inning=1).aggregate(Sum('total_runs'))
            c7 = Deliveries.objects.filter(del_id_id=i.match_id, inning=2).aggregate(Sum('total_runs'))
            c8 = Deliveries.objects.filter(del_id_id=i.match_id, inning=1).exclude(player_dismissed='').aggregate(
                Count('player_dismissed'))
            c9 = Deliveries.objects.filter(del_id_id=i.match_id, inning=2).exclude(player_dismissed='').aggregate(
                Count('player_dismissed'))
            i.inn1 = c6['total_runs__sum']
            i.inn2 = c7['total_runs__sum']
            i.w1 = c8['player_dismissed__count']
            i.w2 = c9['player_dismissed__count']
        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\IPL\iplweb\templates\matches_display.html",
            context={
                "match":c,
                'name':request.user.username,
                "season":kwargs['year'],
            }
        )

class Deliveries_view(LoginRequiredMixin,View):
    login_url = '/login/'
    def get(self,request,**kwargs):
        c = Deliveries.objects.filter(del_id_id = kwargs['mid'])
        c1 = Matches.objects.filter(match_id = kwargs['mid'])
        c2 = Deliveries.objects.filter(del_id_id=kwargs['mid']).values('batsman').annotate(score=Sum('total_runs')).order_by(
            '-score')[:3]

        c3 = Deliveries.objects.exclude(player_dismissed="").filter(del_id_id=kwargs['mid']).values('bowler').annotate(
            ct=Count('player_dismissed')).order_by("-ct")[:3]
        c4 = Deliveries.objects.filter(del_id_id=kwargs['mid'], inning=1).aggregate(wd=Sum('wide_runs'), bye=Sum('bye_runs'), lb = Sum('legbye_runs'), nb = Sum(
            'noball_runs'), pn = Sum('penality_runs'))
        c5 = Deliveries.objects.filter(del_id_id=kwargs['mid'], inning=2).aggregate(wd=Sum('wide_runs'),bye=Sum('bye_runs'),lb=Sum('legbye_runs'), nb=Sum(
                'noball_runs'), pn=Sum('penality_runs'))

        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\IPL\iplweb\templates\match_summary.html",
            context={
                "summary":c,
                "match":c1[0],
                "top_scorers":c2,
                "top_bowlers":c3,
                "extras":c4,
                "extras2":c5,
            }
        )



class login_user(forms.Form):
    username = forms.CharField(
        widget=forms.TextInput(attrs={'class':'input stat','placeholder':'Enter username'}),
        max_length=75,
        required=True
    )
    password = forms.CharField(
        widget=forms.PasswordInput(attrs={'class':'input stat','placeholder':'Enter password'}),
        max_length=75,
        required=True
    )

class LoginController(View):
    def get(self,request,**kwargs):
        if request.user.is_authenticated:
            return redirect('listsView')
        login1 = login_user()
        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\IPL\iplweb\templates\loginform.html",
            context={
                'form':login1,
            }
        )
    def post(self,requests,**kwargs):
        username = requests.POST['username']
        password = requests.POST['password']
        user = authenticate(requests,username = username,password = password)
        if user is not None:
            login(requests,user)
            return redirect("listsView")

class signup_user(forms.Form):
    first_name = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'input stat', 'placeholder': 'Enter firstname'}),
        max_length=75,
        required=True
    )
    last_name = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'input stat', 'placeholder': 'Enter lastname'}),
        max_length=75,
        required=True
    )
    username = forms.CharField(
        widget=forms.TextInput(attrs={'class': 'input stat', 'placeholder': 'Enter username'}),
        max_length=75,
        required=True
    )
    password = forms.CharField(
        widget=forms.PasswordInput(attrs={'class': 'input stat', 'placeholder': 'Enter password'}),
        max_length=75,
        required=True
    )

class SignupController(View):
    def get(self,request,**kwargs):
        if request.user.is_authenticated:
            return redirect('listsView')
        signup1 = signup_user()
        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\IPL\iplweb\templates\signupform.html",
            context={
                'form' : signup1,
            }
        )
    def post(self,request,**kwargs):
        form = signup_user(request.POST)
        if form.is_valid():
            user = User.objects.create_user(**form.cleaned_data)
            user.save()
            if user is not None:
                login(request,user)
                return redirect('listsView')


def Logout(requests):
    logout(requests)
    return redirect('log_con')

class pointsTableView(View):
    def get(self,request,**kwargs):
        c = Matches.objects.filter(season = kwargs['year']).values('winner').annotate(Count('winner')).order_by('-winner__count')
        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\IPL\iplweb\templates\pointsTable.html",
            context={
                "points":c,
            }
        )


class TeamPageView(View):
    def get(self,request,**kwargs):
        if kwargs:
            c = Matches.objects.filter(Q(season=kwargs['year']) & Q(Q(team1=kwargs['name']) | Q(team2=kwargs['name'])))
        else:
            c = Matches.objects.filter(Q(season=2019) & Q(Q(team1="Mumbai Indians") | Q(team2="Mumbai Indians")))
            kwargs["name"] = "Mumbai Indians"
        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\IPL\iplweb\templates\TeamPage.html",
            context={
                "stats":c,
                "team": kwargs["name"],
            }
        )

def home(request):
    c = Matches.objects.filter(season = 2019).order_by('-match_id')
    return render(request, r'C:\MRNDsummer\Precourse\IPL\iplweb\templates\Home.html', {"recentmatches":c,})