FROM python

COPY src/python .
COPY requirements.txt .

RUN pip3 install -r requirements.txt
CMD python3 app.py
