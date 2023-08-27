FROM alpine:3.18.2
RUN apk add g++
RUN apk add doxygen
RUN apk add make
COPY . .
RUN make
CMD ./bin/timetablegen