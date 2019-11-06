all: build-docker run-docker

build-docker:
	docker-compose build 

run-docker:
	docker-compose up
