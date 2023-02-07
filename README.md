# README #

Repository for example C logApp demo. This app show device_ID and a preconfigured message in logs with a time
of refresh configured in appConfig.

AppConfig:

```
{
  "refresh":"10"
}
```

It's necessary to put the refresh time (seconds) like "String".

To future C code implementations, remember to use:
```
printf(&message[0]);
fflush(stdout);
```

to avoid buffering messages to stdout.
