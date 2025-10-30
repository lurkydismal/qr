FROM alpine:latest

WORKDIR /build

RUN apk add \
    binutils clang mold

COPY . .

RUN ./build_release.sh
