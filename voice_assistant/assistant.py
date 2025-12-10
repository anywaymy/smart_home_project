import requests
import speech_recognition as sr

class VoiceAssistant:
    def __init__(self):
        self.r = sr.Recognizer()
        self.api_url = f"http://127.0.0.1:8000/api/v1/devices/1/"

    def run(self):
        print("Привет, я ваш голосовой ассистент")
        print("Скажите 'включи свет' или 'выключи свет'")
        with sr.Microphone() as source:
            while True:
                try:
                    print("\nСлушаю...")
                    audio = self.r.listen(source, timeout=6)
                    text = self.r.recognize_google(audio, language="ru-RU")

                    print(f"Вы сказали: {text.lower()}")

                    if "включи свет" == text.lower():
                        self.control_light("on")
                    elif "выключи свет" == text.lower():
                        self.control_light("off")
                    elif "завершение работы" == text.lower():
                        print("Выход...")
                        break

                except sr.WaitTimeoutError:
                    continue
                except sr.UnknownValueError:
                    print("Не понял")
                except Exception as e:
                    print(f"Ошибка: {e}")


    def control_light(self, status):
        try:
            response = requests.patch(
                self.api_url,
                json={
                    'status': status,
                },
                headers={"Content-Type": "application/json"},
                timeout=5
            )

            if response.status_code == 200:
                print(f"Свет: {status}")
            else:
                print(f"Ошибка: {response.status_code}")

        except Exception as e:
            print(f"Ошибка подключения: {e}")


if __name__ == "__main__":
    assistant = VoiceAssistant()
    assistant.run()
