# README #

Repository for example C logApp demo. This app show device_ID and a preconfigured message in logs with a time
of refresh configured in appConfig.

AppConfig:

```
{
  "refresh": 10
}
```

It's necessary to put the refresh time (seconds) like "String".

To future C code implementations, remember to use:
```
printf(&message[0]);
fflush(stdout);
```

to avoid buffering messages to stdout.

To build this app, Docker and Docker Compose are needed.

Modify "platform" inside docker-compose.yaml to "linux/amd64", "linux/arm64" or "linux/arm/v7" depending on your platform.

To build the app, run:
```
docker compose up --build
```

Built app will be in "output" folder.