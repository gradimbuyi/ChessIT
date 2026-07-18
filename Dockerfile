FROM python:3.11-slim

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY Makefile .
COPY include/ include/
COPY src/ src/

RUN make

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY client.py .

CMD ["python", "client.py"]