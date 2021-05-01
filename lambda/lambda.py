import requests
import logging
import time
import json
import uuid
import os

# Configuration
DEBUG_MODE = False
FIBER_URL = os.environ['FIBER_URL']
FIBER_KEY = os.environ['FIBER_KEY']

# Logger
logger = logging.getLogger()
logger.setLevel(logging.INFO)

# Main Handler
def lambda_handler(request, context):
    logger.info('Handling IoT request')

    try:
        if DEBUG_MODE:
            logger.info('Directive:')
            logger.info(json.dumps(request, indent=4, sort_keys=True))

        directive_ver = get_directive_version(request)
        if directive_ver != "3":
            logger.error(f'Cannot handle directive version {directive_ver}')
            return 

        
        response = requests.post(FIBER_URL, headers={'Authorization': f'X-Fiber-Auth {FIBER_KEY}'}, json=request)

        if response.status_code != requests.codes.ok:
            logger.error("Failed to call fiber")
            return
        
        responseJson = response.json()

        if DEBUG_MODE:
            logger.info('Response:')
            logger.info(json.dumps(responseJson, indent=4, sort_keys=True))

        return responseJson
    except ValueError as error:
        logger.error(error)
        raise

# Alexa parser
def get_directive_version(request):
    try:
        return request["directive"]["header"]["payloadVersion"]
    except:
        try:
            return request["header"]["payloadVersion"]
        except:
            return "-1"