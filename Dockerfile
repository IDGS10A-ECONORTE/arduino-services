from python:3.14.0rc3-alpine3.22
WORKDIR /flask
COPY "requirements.txt" .
RUN pip install -r "requirements.txt"
COPY . .
EXPOSE 7777
CMD [ "python3", "main.py" ]