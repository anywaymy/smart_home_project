from django.db import models


class Device(models.Model):
    STATUS_CHOICE = [
        ('on', 'Включено'),
        ('off', 'Выключено'),
        ('unknown', 'Неизвестно'),
        ('offline', 'Не в сети'),
    ]

    name = models.CharField(max_length=100, verbose_name="Название устройства")
    ip_address = models.GenericIPAddressField(verbose_name="IP адрес")
    status = models.CharField(
        max_length=10,
        choices=STATUS_CHOICE,
        default="unknown",
        verbose_name="Статус"
    )

    created_at = models.DateTimeField(auto_now_add=True, verbose_name="Создано")

    class Meta:
        verbose_name = "Устройство"
        verbose_name_plural = "Устройства"
        ordering = ['name']

    def __str__(self):
        return f"{self.name}"

