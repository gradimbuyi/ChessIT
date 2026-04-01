import os

from dotenv import load_dotenv

load_dotenv()

API_TOKEN = os.getenv("API_TOKEN")

BOT_USERNAME = os.getenv("BOT_USERNAME")

BASE_URL = "https://lichess.org/api"

HEADERS = {
    "Authorization": f"Bearer {API_TOKEN}"
}