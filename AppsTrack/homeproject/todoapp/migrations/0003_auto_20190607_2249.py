# Generated by Django 2.2.1 on 2019-06-07 17:19

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('todoapp', '0002_auto_20190607_2239'),
    ]

    operations = [
        migrations.AlterField(
            model_name='todoitem',
            name='due_date',
            field=models.DateTimeField(auto_now_add=True),
        ),
    ]
