from django.shortcuts import render
from django.http import HttpResponse
from django.shortcuts import render
import onlineapp

import os
import django
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'classproject.settings')
django.setup()
from onlineapp.models import *
# Create your views here.
def hello_world(request):
    fp = open('C:\MRNDsummer\Precourse\classproject\onlineapp\hello_1.html','r')
    l = fp.readlines()
    print(l)
    return HttpResponse(l)


def get_all_colleges(request):
    d = College.objects.all()
    res=''
    for i in d:
        res+=str(i.name)+"------------->"+str(i.acronym)+'<br>'
    return HttpResponse(res)


def college_student_info(request):
    v = request.path
    v = v[:-1]
    d1 = v[-1]
    d2 = v[-2]
    if(d2!='/'):
        d = int(d2)*10+int(d1)
    else:
        d = d1
    df = Student.objects.filter(college__id=d)
    return render(
        request,
        template_name=r"C:\MRNDsummer\Precourse\classproject\onlineapp\templates\acronym.html",
        context={
            'jails': df,
            'title': df[0].college,
        }
    )