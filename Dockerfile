FROM python:3.11-slim

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY Makefile .
COPY engine/ engine/
RUN make

COPY requirements.txt .
RUN pip install -r requirements.txt

COPY client/ client/

CMD ["python", "-m", "client.src.main"]