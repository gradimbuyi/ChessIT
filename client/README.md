# Lichess Client

The client applicaiton talks to Lichess using their public facing APIs. These API allows the bot to accept challenges, deny challenges, and play the best moves calculated by the engine.

## How to Use

The application is written in Python and depends on libaries such as requests and dotenv. In order for the client to run, theses dependencies need to be installed first.

### Python Virtual Environment Setup

These instructions assume that the developer is running these commands at the root directory. It also assume that the developer is using a Unix/Linux machine.

1. Create Virtual Enviornment:

```
$ python -m venv .venv
```

2. Activate Virtual Environment:

```
$ source .venv/bin/activate
```

3. Installing the dependencies

```
$ pip install -r requirements.txt
```

### Running the Client

Launching the program:

```
$ python -m client.src.main
```
