-----------------------------80589521833606600364177121478
Content-Disposition: form-data; name="filePost"; filename="Dockerfile"
Content-Type: application/octet-stream

FROM ubuntu:23.10

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    python3 \
    python3-pip \
    python3-venv \
    && rm -rf /var/lib/apt/lists/*

COPY ../. app

WORKDIR app

RUN make all

RUN python3 -m venv venv && \
    source venv/bin/activate && \
    pip install requests pytest

RUN chmod +x tests/run-tests.sh

CMD ["./tests/run-tests.sh"]

-----------------------------80589521833606600364177121478--
