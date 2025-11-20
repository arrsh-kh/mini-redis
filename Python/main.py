import json

database = {}

def SET(key: str, value: str):
    """
    SET(key: str, value: str)
    Sets a new entry in the database
    """
    database[key] = value
    print("OK")

def GET(key: str):
    """
    GET(key: str)
    Gets the value of an entry in the database
    """
    print(database.get(key, "(nil)"))

def INCR(key: str):
    """
    INCR(key: str)
    Increases the numeric value of an entry by 1
    """
    database[key] = int(database.get(key, 0)) + 1
    print(database.get(key))

def DEC(key: str):
    """
    DEC(key: str)
    Decreases the numeric value of an entry by 1
    """ 
    database[key] = database.get(key, 0) - 1
    print(database.get(key))

def DEL(key: str):
    """
    DEL(key: str)
    Deletes an entry in the database
    """
    removed = 1 if key in database else 0
    database.pop(key, None)
    print(f"({removed})")

def SAVE():
    """
    SAVE()
    Saves the databse as a JSON locally
    """
    with open("dump.json", "w") as f:
        json.dump(database, f, indent=4)
    print(f"OK (wrote {len(database)} keys to dump.json)")

def main():

    valid_instructions = ["SET", "GET", "INCR", "DEC", "DEL", "SAVE"]

    while True:
        user_input = input("miniredis> ")
        keys = user_input.split()

        match keys[0]:
            case "SET":
                SET(keys[1], keys[2])
                continue
            case "GET":
                GET(keys[1])
                continue
            case "INCR":
                INCR(keys[1])
                continue
            case "DEC":
                DEC(keys[1])
                continue
            case "DEL":
                DEL(keys[1])
                continue
            case "SAVE":
                SAVE()
                continue
            case "EXIT":
                return print("bye")
            case _:
                print(f"Enter a valid instruction {valid_instructions}")
                continue

main()
