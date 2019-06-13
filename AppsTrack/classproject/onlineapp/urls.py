from django.urls import path
from onlineapp.view2.college import College_view
from django.urls import path
from django.urls import re_path
from onlineapp import views

from onlineapp.view2.college import *
from .method_views import *
urlpatterns = [
    # path('onlineapp/hello/',views.hello_world),
    # path('onlineapp/get_all_colleges/',views.get_all_colleges),
    # re_path(r'^onlineapp/college_student_info/\d+/',views.college_student_info),
    path('colleges/',College_view.as_view(),name = "colleges_html"),
    path('colleges/<int:pk>/',College_view.as_view(),name='colleges_id'),
    path('colleges/<int:pk>/edit',AddCollegeView.as_view(),name = "edit_college"),
    path('colleges/<int:pk>/delete',AddCollegeView.as_view(),name = "delete_college"),
    path('colleges/add',AddCollegeView.as_view(),name = "add_college_view"),
    path('colleges/<int:pk>/add',AddStudentView.as_view(),name="std_add"),
    path('colleges/<int:pk>/<str:name>/delete1',AddStudentView.as_view(),name = "delete_std"),
    path('colleges/<int:pk>/<str:name>/edit',AddStudentView.as_view(),name = "edit_std"),
    path('login/',LoginController.as_view(),name = 'log_con'),
    path('signup/',SignupController.as_view(),name = 'sign_con'),
    path(r'logout/',Logout,name='logout_con'),
    path(r'api/colleges/',rest_college),
    path(r'api/colleges/<int:cpk>/',rest_college),
    path(r'api/colleges/<int:cpk>/edit/',rest_college),
    path(r'api/colleges/<int:cpk>/delete/',rest_college),
    path(r'api/colleges/<int:pk>/students/',StudentSerializerView.as_view()),
    path(r'api/colleges/<int:pk>/students/<int:sk>', StudentSerializerView.as_view())
]