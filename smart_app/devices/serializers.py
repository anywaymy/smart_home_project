from rest_framework import serializers
from .models import Device

class DeviceSerializer(serializers.ModelSerializer):
    class Meta:
        model = Device
        fields = ('id','name','ip_address','status','created_at')
        read_only_fields = ('created_at',)

class ControlSerializer(serializers.Serializer):
    action = serializers.ChoiceField(choices=('on','off','status'))