from django.views import View
from onlineapp.models import *
from django.shortcuts import render,redirect
from django.contrib.auth.models import Permission
from onlineapp.forms import *
from django import forms
from django.contrib.auth import authenticate, login, logout
from django.urls import resolve
from django.contrib.auth.models import User

from django.contrib.auth.backends import ModelBackend
from django.contrib.auth.mixins import LoginRequiredMixin
class College_view(LoginRequiredMixin,View):
    login_url = '/login/'
    def get(self,request,**kwargs):
        if kwargs:
            df = Student.objects.filter(college__id=kwargs['pk'])
            f = request.user.get_all_permissions()
            df1 = MockTest.objects.values('student__name','student__email','total','student__college','student_id').filter(student__college__id = kwargs['pk']).order_by('-total')
            return render(
                request,
                template_name=r"C:\MRNDsummer\Precourse\classproject\onlineapp\templates\acronym.html",
                context={
                    'jails':df1,
                    'title':df[0].college,
                    'id':kwargs['pk'],
                    'permissions': f,
                }
            )
        c = College.objects.all()
        return render(
            request,
            template_name="C:/MRNDsummer/Precourse/classproject/onlineapp/templates/colleges.html",
            context={
                "jails":c,
                'permissions': request.user.get_all_permissions,
            }
        )



class AddCollegeView(LoginRequiredMixin,View):
    login_url = '/login/'
    def get(self,request,**kwargs):
        college = AddCollege()
        if kwargs:
            c = College.objects.get(**kwargs)
            college = AddCollege(instance=c)
        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\classproject\onlineapp\templates\forms.html",
            context={
                'form' : college,
                'permissions': request.user.get_all_permissions,
            }
        )

    def post(self,request,**kwargs):
        if resolve(request.path_info).url_name == 'delete_college':
            College.objects.get(pk=kwargs.get('pk')).delete()
            return redirect('colleges_html')
        if resolve(request.path_info).url_name == 'edit_college':
            college = College.objects.get(pk = kwargs.get('pk'))
            form = AddCollege(request.POST,instance=college)
            if form.is_valid():
                form.save()
                return redirect('colleges_html')

        if request.method == "POST":
            form = AddCollege(request.POST)
            if form.is_valid():
                topic = form.save(commit=False)
                topic.save()
            return redirect('colleges_html')


class AddStudentView(LoginRequiredMixin,View):
    login_url = '/login/'
    def get(self, request, **kwargs):
        student = AddStudent()
        mock = AddMockTest()
        if len(kwargs) > 1:
            df = Student.objects.get(db_folder = kwargs['name'])
            df1 = MockTest.objects.get(student_id = kwargs['name'])
            student = AddStudent(instance=df)
            mock = AddMockTest(instance=df1)

        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\classproject\onlineapp\templates\studentForm.html",
            context={
                'form': student,
                'form1':mock,
                'permissions': request.user.get_all_permissions,
            }
        )

    def post(self,request,**kwargs):
        if resolve(request.path_info).url_name == 'delete_std':
            Student.objects.get(db_folder=kwargs.get('name')).delete()
            return redirect('colleges_id', pk=kwargs['pk'])
        if resolve(request.path_info).url_name == 'edit_std':
            student = Student.objects.get(db_folder = kwargs['name'])
            form = AddStudent(request.POST, instance=student)
            if form.is_valid():
                form.save()
            mock = MockTest.objects.get(student_id = kwargs['name'])
            form = AddMockTest(request.POST, instance=mock)
            if form.is_valid():
                f = form.save(commit=False)
                f.total = int(form['problem1'].data) + int(form['problem2'].data) + int(form['problem3'].data) + int(form['problem4'].data)
                f.save()
            return redirect('colleges_id', pk=kwargs['pk'])
        if request.method == "POST":
            name=''
            folder = ''
            form = AddStudent(request.POST)
            if form.is_valid():
                acr = College.objects.values('acronym').filter(id = kwargs['pk'])
                name = form['name'].data
                mail = form['email'].data
                folder = form['db_folder'].data
                acry = acr[0]
                acry = acry['acronym']
                c = Student(name = name,email = mail,db_folder = folder,college_id = acry)
                c.save()
            form = AddMockTest(request.POST)
            if form.is_valid():
                total = int(form['problem1'].data) + int(form['problem2'].data) + int(form['problem3'].data) + int(form['problem4'].data)
                c = MockTest(problem1 = int(form['problem1'].data),problem2 = int(form['problem2'].data),problem3 = int(form['problem3'].data),problem4 = int(form['problem4'].data),total = total,student_id = folder)
                c.save()
        return redirect('colleges_id',pk=kwargs['pk'])

class SignupController(View):
    def get(self,request,**kwargs):
        if request.user.is_authenticated:
            return redirect('colleges_html')
        signup1 = signup_user()
        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\classproject\onlineapp\templates\signupform.html",
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
                return redirect('colleges_html')

class LoginController(View):
    def get(self,request,**kwargs):
        if request.user.is_authenticated:
            return redirect('colleges_html')
        login1 = login_user()
        return render(
            request,
            template_name=r"C:\MRNDsummer\Precourse\classproject\onlineapp\templates\loginform.html",
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
            return redirect("colleges_html")

def Logout(requests):
    logout(requests)
    return redirect('log_con')