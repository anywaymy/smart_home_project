from django.contrib import admin
from .models import Device

@admin.register(Device)
class DeviceAdmin(admin.ModelAdmin):
    list_display = ('name', 'ip_address', 'status', 'created_at')
    list_filter = ('status', 'created_at')
    search_fields = ('name', 'ip_address')
    list_editable = ('status',)
