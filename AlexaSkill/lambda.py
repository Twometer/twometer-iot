import logging
import time
import json
import uuid
import requests

RELAIS_SERVER = "https://fibers.twometer.de/twometer-iot/publish/"
RELAIS_SERVER_AUTH = "X-FiberAuth $token$"

logger = logging.getLogger()
logger.setLevel(logging.INFO)

def lambda_handler(request, context):
    logger.info("Lambda was called")

    try:
        logger.info("Directive:")
        logger.info(json.dumps(request, indent=4, sort_keys=True))

        directive_ver = get_directive_version(request)
        if directive_ver != "3":
            logger.error("Cannot handle directive V" + directive_ver)
        else:
            response = handle_directive(request)

        if response.status_code != requests.codes.ok:
            logger.error("Failed to call relay")
            return
        
        responseJson = response.json()

        logger.info("Response:")
        logger.info(json.dumps(responseJson, indent=4, sort_keys=True))
        return responseJson
    except ValueError as error:
        logger.error(error)
        raise

def handle_directive(request):
    return requests.post(RELAIS_SERVER, headers= {'Authorization': RELAIS_SERVER_AUTH}, json=request)

def get_directive_version(request):
    try:
        return request["directive"]["header"]["payloadVersion"]
    except:
        try:
            return request["header"]["payloadVersion"]
        except:
            return "-1"