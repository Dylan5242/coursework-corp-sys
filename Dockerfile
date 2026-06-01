FROM debian:bookworm-slim AS builder

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY CMakeLists.txt ./
COPY include ./include
COPY src ./src
COPY tests ./tests
COPY examples ./examples

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel

FROM builder AS test

RUN ctest --test-dir build --output-on-failure

CMD ["ctest", "--test-dir", "build", "--output-on-failure"]

FROM debian:bookworm-slim AS runtime

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY --from=test /src/build/docgen_lite /usr/local/bin/docgen_lite
COPY --from=test /src/examples /examples

VOLUME ["/output"]

ENTRYPOINT ["docgen_lite"]
CMD ["/examples", "/output", "auto", "advanced"]
