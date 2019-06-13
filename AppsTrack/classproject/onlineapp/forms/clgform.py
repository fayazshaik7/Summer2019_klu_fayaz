from django import forms
from onlineapp.models import *
class AddCollege(forms.ModelForm):
    class Meta:
        model = College
        exclude = ['id']
        widgets = {
            'name' : forms.TextInput(attrs={'class':'input stat','placeholder':'Enter name'}),
            'location' : forms.TextInput(attrs={'class':'input stat','placeholder':'Enter location'}),
            'acronym' : forms.TextInput(attrs={'class':'input stat','placeholder':'Enter acronym'}),
            'contact' : forms.EmailInput(attrs={'class':'input stat','placeholder':'Enter contact mail'})
        }

class AddStudent(forms.ModelForm):
    class Meta:
        model = Student
        exclude = ['college','dob']
        widgets = {
            'name' : forms.TextInput(attrs={'class':'input stat','placeholder':'Enter name'}),
            'email' : forms.EmailInput(attrs={'class':'input stat','placeholder':'Enter email'}),
            'db_folder' : forms.TextInput(attrs={'class':'input stat','placeholder':'Enter db folder'}),
            # 'dropped_out' : forms.BooleanField()
        }

class AddMockTest(forms.ModelForm):
    class Meta:
        model = MockTest
        exclude = ['total','student']
        widgets = {

        }

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