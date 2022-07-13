import os
import random


def generate_nickname():
    nickNumber = random.randint(0, 86770)

    with open('../set_generators/usernames.txt', 'r') as file:
        for line in file:
            nickNumber -= 1
            if nickNumber <= 0:
                return line.rstrip()

def generate_ip():
    ipLst = [str(random.randint(100, 250)) for i in range(0, 4)]
    return '.'.join(ipLst)

def generate_port():
    return random.randint(5, 65535)

#=======================================================================

def generate_container(mitreSubType, symptomType, ip, port, user, command):
    container = {}

    if mitreSubType:
        container['mitre_subtype'] = mitreSubType
    if symptomType:
        container['type'] = symptomType
    if ip:
        container['addr_ip'] = ip
    if port:
        container['port'] = port
    if user:
        container['username'] = user
    if command:
        container['command'] = command

    return container

def visual_basic_generate(user):
    return generate_container('visual_basic', 'run_script', None, None, user, 'vbs')


def ingress_tool_transfer_generate(user, ip):
    commandLst = ['curl', 'scp', 'sftp', 'rsync', 'finger', 'wget']
    command = commandLst[random.randint(0, len(commandLst) - 1)]

    return generate_container('ingress_tool_transfer', 'transfer_tool', ip, None, user, command)


def spearphishing_attachment_generate(ip):
    return generate_container('spearphishing_attachment', 'email_letter', ip, None, None, 'attachment')


def malicious_file_generate(user):
    return generate_container('malicious_file', 'run_file', None, None, user, 'executable_file')


def native_api_generate(user):
    apiCalls = ['fork', 'create_proccess']
    apiCall = apiCalls[random.randint(0, len(apiCalls) - 1)]

    return generate_container('native_api', 'start_new_thread', None, None, user, apiCall)


def process_injection_generate(user):
    return generate_container('process_injection', 'code_injection', None, None, user, 'proccess_inject')


def registry_run_keys___startup_folder_generate(user):
    return generate_container('registry_run_keys___startup_folder', 'edit_params', None, None, user, 'registry_run_keys')

def dynamic_data_exchange_generate(user):
    return generate_container('dynamic_data_exchange', 'data_exchange', None, None, user, 'dde')


def process_discovery_generate(user):
    procDiscoveryCommands = ['ps', 'Get-Process', 'CreateToolhelp32Snapshot']
    procDiscoverEnvs = ['proc_discovery_cmd', 'proc_discovery_api']

    procDiscoveryCommand = procDiscoveryCommands[random.randint(0, len(procDiscoveryCommands) - 1)]
    procDiscoverEnv = procDiscoverEnvs[random.randint(0, len(procDiscoverEnvs) - 1)]

    return generate_container('process_discovery', procDiscoverEnv, None, None, user, procDiscoveryCommand)

def spearphishing_link_generate(user, ipAddr):
    return generate_container('spearphishing_link', 'malicious_url', ipAddr, None, user, None)

def code_signing_generate(user):
    return generate_container('code_signing', 'change_file_permission', None, None, user, None)

def scheduled_task_generate(user):
    schedulers = ['cron', 'task_manager']
    scheduler = schedulers[random.randint(0, len(schedulers) - 1)]

    return generate_container('scheduled_task', 'scheduler', None, None, user, scheduler)

def malicious_link_generate(user, ipAddr):
    return generate_container('malicious_link', 'malicious_url', ipAddr, None, user, None)

def javascript_generate(user):
    jsEnvs = ['jscript', 'jxa']
    jsEnv = jsEnvs[random.randint(0, len(jsEnvs) - 1)]

    return generate_container('javascript', 'js_file_execution', None, None, user, jsEnv)

def powershell_generate(user):
    return generate_container('powershell', 'shell_environment', None, None, user, 'powershell')

def credentials_from_web_browsers_generate(user):
    return generate_container('credentials_from_web_browsers', 'read_credential_file', None, None, user, None)

def network_share_discovery_generate(user):
    return generate_container('network_share_discovery', 'share_discovery', None, None, user, 'smb')

def tool_generate(user):
    tools = ['psexec', 'tor', 'mimicatz', 'imminent_monitor']
    tool = tools[random.randint(0, len(tools) - 1)]

    return generate_container('tool', 'specific_tool', None, None, user, tool)

def windows_command_shell_generate(user):
    return generate_container('windows_command_shell', 'shell_environment', None, None, user, 'cmd')

def system_network_configuration_discovery_generate(user):
    commands = ['ipconfig', 'arp', 'nbstat', 'route']
    command = commands[random.randint(0, len(commands) - 1)]

    return generate_container('system_network_configuration_discovery', 'network_configuration', None, None, user, command)

def archive_via_utility_generate(user):
    archives = ['tar', 'zip', '7z']
    archive = archives[random.randint(0, len(archives) - 1)]

    return generate_container('archive_via_utility', 'compress_tool', None, None, user, archive)

def pass_the_hash_generate(user):
    return generate_container('pass_the_hash', 'hash_auth', None, None, user, None)

def malware_generate(user):
    return generate_container('malware', 'run_file', None, None, user, 'malware')

def remote_desktop_protocol_generate(user, ipAddr, port):
    return generate_container('remote_desktop_protocol', 'auth_rdp', ipAddr, port, user, 'rdp')

def system_service_discovery_generate(user):
    commands = ['systemctl', 'net']
    command = commands[random.randint(0, len(commands) - 1)]

    return generate_container('system_service_discovery', 'system_configuration', None, None, user, command)

def local_account_generate(user):
    commands = ['id', 'groups', 'whoami']
    command = commands[random.randint(0, len(commands) - 1)]

    return generate_container('local_account', 'system_configuration', None, None, user, command)

def system_network_connections_discovery_generate(user):
    commands = ['netstat', 'lsof']
    command = commands[random.randint(0, len(commands) - 1)]

    return generate_container('system_network_connections_discovery', 'network_configuration', None, None, user, command)
