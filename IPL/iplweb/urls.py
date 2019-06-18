from .views import *
from django.urls import path
urlpatterns = [
    path(r'',home),
    path(r'tm/',teams),
    path('matches/',Matches_view.as_view(),name = "listsView"),
    path('matches/<int:year>/',Matches_view.as_view(),name = "listsView1"),
    path('match_report/<int:mid>/',Deliveries_view.as_view(),name = "matchsummary"),
    path('login/', LoginController.as_view(), name='log_con'),
    path('signup/', SignupController.as_view(), name='sign_con'),
    path(r'logout/', Logout, name='logout_con'),
    path(r'pointsTable/<int:year>/',pointsTableView.as_view(),name="ptTable"),
    path(r'TeamStats/<str:name>/<int:year>/',TeamPageView.as_view(),name = "teampg"),

]