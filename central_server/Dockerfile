FROM python:3
ENV PYTHONUNBUFFERED 1
RUN mkdir /code
WORKDIR /code
COPY requirements.txt /code/
RUN pip install -r requirements.txt
COPY ./_utils/wait-for-it.sh /usr/bin/wait-for-it
RUN chmod a+xr /usr/bin/wait-for-it
COPY . /code/