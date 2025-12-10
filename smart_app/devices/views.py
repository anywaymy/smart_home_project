import requests

from rest_framework import viewsets
from rest_framework.response import Response
from rest_framework.permissions import AllowAny

from .models import Device
from .serializers import DeviceSerializer


class DeviceViewSet(viewsets.ModelViewSet):
    queryset = Device.objects.all()
    serializer_class = DeviceSerializer
    permission_classes = (AllowAny,)

    def update(self, request, *args, **kwargs):
        device = self.get_object()

        if "status" in request.data:
            new_status = request.data["status"]

            if new_status in ("on", "off"):
                try:
                    response = requests.get(
                        f"http://{device.ip_address}/light/{new_status}",
                        timeout=3
                    )

                    if response.status_code != 200:
                        return Response({
                            "error": f"ESP вернул ошибку {response.status_code}"
                        }, status=500)

                except Exception as e:
                    device.status = "offline"
                    device.save()

                    return Response({
                        "error": f"Не удалось подключиться к ESP: {str(e)}",
                        "status": "offline"
                    }, status=200)

        return super().update(request, *args, **kwargs)

