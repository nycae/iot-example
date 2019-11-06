FROM python
COPY src/python .
COPY requirements.txt .
RUN chmod +x app.py
RUN pip3 install -r requirements.txt
CMD ./app.py
