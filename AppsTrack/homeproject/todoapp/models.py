from django.db import models

# Create your models here.
class todolist(models.Model):
    name = models.CharField(max_length=128,unique=True)
    created_at = models.DateTimeField(auto_now_add=True)
    def __str__(self):
        return self.name

class todoitem(models.Model):
    description = models.CharField(max_length=128)
    due_date = models.DateTimeField(auto_now_add=True)
    completed = models.BooleanField(default=False)
    todolist = models.ForeignKey(todolist,on_delete=models.CASCADE,to_field='name')
    def __str__(self):
        return self.description
