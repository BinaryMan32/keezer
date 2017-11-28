FROM arm32v7/python

ENV PYTHONPATH=/src

COPY requirements.txt /src/requirements.txt
RUN pip install --no-cache-dir -r /src/requirements.txt

COPY . /src

RUN cd /usr/local/lib/python3.6/site-packages && python /src/setup.py develop
# RUN cd /src && python setup.py develop

CMD keezer
