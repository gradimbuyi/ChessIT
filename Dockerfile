FROM python:3.11-slim

RUN apt-get update && apt-get install -y g++ cmake make && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt .
COPY include/ include/
COPY src/ src/

RUN cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build/Release

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY client.py .

CMD ["python", "client.py"]