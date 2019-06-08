from django.contrib import admin
from .models import College,Student,MockTest,Teacher
# Register your models here.
admin.site.register(College)
admin.site.register(Student)
admin.site.register(MockTest)
admin.site.register(Teacher)