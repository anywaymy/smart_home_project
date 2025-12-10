from django.contrib import admin
from django.urls import path, include
from rest_framework.routers import DefaultRouter
from devices.views import DeviceViewSet

router = DefaultRouter()
router.register(r'devices', DeviceViewSet)


urlpatterns = [
    path('admin/', admin.site.urls),
    path('api/v1/', include(router.urls)),
]
