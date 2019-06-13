from django.views import View
from django.views.decorators.csrf import csrf_exempt
from rest_framework.decorators import api_view, permission_classes
from rest_framework.response import Response

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
from django.shortcuts import get_object_or_404
from rest_framework.decorators import api_view,APIView
from rest_framework import status, permissions
from .serializers import *
@api_view(['GET', 'POST','PUT','DELETE' ])
@csrf_exempt
@permission_classes((permissions.AllowAny,))
def rest_college(request,**kwargs):
    if len(kwargs)>0:
        if request.method=="GET":
            c = College.objects.get(id=kwargs['cpk'])
            serial = CollegeSerializer(c)
            return Response(serial.data, status=status.HTTP_200_OK)
    if request.method == "GET":
        c = College.objects.all()
        serial = CollegeSerializer(c,many=True)
        return Response(serial.data, status=status.HTTP_200_OK)
    if request.method == "POST":
        serial = CollegeSerializer(data=request.data)
        if serial.is_valid():
            serial.save()
            return Response(serial.data, status=status.HTTP_201_CREATED)
        return Response(serial.data, status=status.HTTP_400_BAD_REQUEST)
    if request.method == "PUT":
        college = College.objects.get(id=kwargs['cpk'])
        serial = CollegeSerializer(college,data = request.data)
        if serial.is_valid():
            serial.save()
            return Response(serial.data, status=status.HTTP_201_CREATED,content_type="application/json")
        return Response(serial.data, status=status.HTTP_400_BAD_REQUEST)
    if request.method == "DELETE":
        College.objects.get(id = kwargs['cpk']).delete()
        return Response({"key":"deleted"},status=status.HTTP_202_ACCEPTED,content_type="application/json")


class StudentSerializerView(APIView):

    def get(self,request,*args,**kwargs):
        college = College.objects.get(id=kwargs.get('pk'))

        if not kwargs.get('sk'):
            student = Student.objects.filter(college__id=college.id).all()
            serializer = StudentSerializer(student, many=True)
        else:
            # student = Student.objects.filter(college_id=college.id).get(id=kwargs.get('sk'))
            student = get_object_or_404(Student, id=kwargs.get('sk'))
            serializer = StudentSerializer(student)

        return Response(serializer.data)

    def post(self,request,*args,**kwargs):
        student = StudentDetailsSerializer(data=request.data,context={'college_id':kwargs.get('pk')})
        if student.is_valid():
            student.save()
            return Response(student.data,status=status.HTTP_201_CREATED)

    def delete(self, request, pk, sk, format=None):
        student = get_object_or_404(Student, pk=sk)
        student.delete()
        return Response(status=status.HTTP_204_NO_CONTENT)

    def put(self, request, pk,sk, format=None):
        student = get_object_or_404(Student,id=sk)
        serializer = StudentSerializer(student, data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

