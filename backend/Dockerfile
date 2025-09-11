FROM python:3.11-slim

WORKDIR /app

RUN apt-get update && apt-get install -y\
    build-essential python3-dev cmake\
    && rm -rf /var/lib/apt/lists/*

COPY requirements.txt .

RUN pip install --no-cache-dir -r requirements.txt

COPY . .

RUN pip install .

EXPOSE 5000

CMD ["python" , "run.py"]
