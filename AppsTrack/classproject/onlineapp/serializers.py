from rest_framework import serializers
from .models import *

class CollegeSerializer(serializers.ModelSerializer):
    class Meta:
        model = College
        fields = ('id','name','location','acronym','contact')

class StudentSerializer(serializers.ModelSerializer):
    class Meta:
        model = Student
        fields = ('id','name','dob','email','db_folder','dropped_out','college')

class MockTestSerializer(serializers.ModelSerializer):
    class Meta:
        model = MockTest
        fields = ('id','problem1','problem2','problem3','problem4')

class StudentDetailsSerializer(serializers.ModelSerializer):
    mocktest = MockTestSerializer(read_only=False,many=False)

    class Meta:
        model = Student
        fields = ('id', 'name', 'dob', 'email', 'db_folder', 'dropped_out','mocktest')

    def create(self,validated_data):
        mock_vals = validated_data.pop('mocktest')
        mock = MockTest(**mock_vals)

        student = Student(**validated_data)
        c =College.objects.filter(id = self.context['college_id']).values('acronym')
        c = c[0]
        c = c['acronym']
        student.college_id = c
        student.save()

        mock.total = 0
        for i in range(1, 5):
            mock.total += int(mock_vals['problem' + str(i)])
        mock.student = student
        mock.save()

        return student

    def update(self, instance, validated_data):
        instance.name = validated_data.get('name', instance.name)
        instance.dob = validated_data.get('dob', instance.dob)
        instance.email = validated_data.get('created', instance.email)
        instance.db_folder = validated_data.get('db_folder',instance.db_folder)
        instance.dropped_out = validated_data.get('dropped_out',instance.dropped_out)
        return instance


