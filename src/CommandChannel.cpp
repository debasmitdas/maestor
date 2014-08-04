/**
 * The hubo-ach command Ach channel. Used for sending commands to the robot. 
 */

#include "CommandChannel.h"

/**
 * List of all the joints
 */
const char *CommandChannel::urdf_joint_names[] = {
    "WST", "NKY", "NK1", "NK2",
    "LSP", "LSR", "LSY", "LEP", "LWY", "LWR", "LWP",
    "RSP", "RSR", "RSY", "REP", "RWY", "RWR", "RWP",
    "UNUSED1",
    "LHY", "LHR", "LHP", "LKP", "LAP", "LAR",
    "UNUSED2",
    "RHY", "RHR", "RHP", "RKP", "RAP", "RAR",
    "RF1", "RF2", "RF3", "RF4", "RF5",
    "LF1", "LF2", "LF3", "LF4", "LF5",
    "unknown1", "unknown2", "unknown3", "unknown4", "unknown5", "unknown6", "unknown7", "unknown8"};

/**
 * Create the command channel 
 */
CommandChannel::CommandChannel(){
    int r = ach_open(&huboBoardCommandChannel, HUBO_CHAN_BOARD_CMD_NAME, NULL);
    if (ACH_OK != r)
        cerr << "Error! Command Channel failed with state " << r << endl;
}

/**
 * Destructor
 */
CommandChannel::~CommandChannel() {}

int CommandChannel::indexLookup(string &joint) {
    if (joint.length() != 3)
        return -1;
    int best_match = -1;

    for (int i = 0; i < HUBO_JOINT_COUNT; i++) {
        if (strcmp(joint.c_str(), urdf_joint_names[i]) == 0)
            best_match = i;
    }
    return best_match;
}

/**
 * Enable a joint
 * @param  joint The joint to enable
 * @return       True on success
 */
bool CommandChannel::enable(string joint){
    BoardCommand command;
    memset(&command, 0, sizeof(command));
    int jointNum = 0;

    if (strcmp(joint.c_str(), "all") == 0){
        command.type = D_CTRL_ON_OFF_ALL;
        command.param[0] = D_ENABLE;
    } else if ((jointNum = indexLookup(joint)) == -1){
        return false;
    } else {
        command.type = D_CTRL_ON_OFF;
        command.joint = jointNum;
        command.param[0] = D_ENABLE;
    }

    int r = ach_put(&huboBoardCommandChannel, &command, sizeof(command));

    if (ACH_OK != r) {
        cerr << "Error! Command enable failed with state " << r << endl;
        return false;
    }
    return true;
}

/**
 * Disable a joint
 * @param  joint The joint you want to disable 
 * @return       True on success
 */
bool CommandChannel::disable(string joint){
    BoardCommand command;
    memset(&command, 0, sizeof(command));
    int jointNum = 0;

    if (strcmp(joint.c_str(), "all") == 0){
        command.type = D_CTRL_ON_OFF_ALL;
        command.param[0] = D_DISABLE;
    } else if ((jointNum = indexLookup(joint)) == -1){
        return false;
    } else {
        command.type = D_CTRL_ON_OFF;
        command.joint = jointNum;
        command.param[0] = D_DISABLE;
    }

    int r = ach_put(&huboBoardCommandChannel, &command, sizeof(command));

    if (ACH_OK != r) {
        cerr << "Error! Command disable failed with state " << r << endl;
        return false;
    }
    return true;
}

/**
 * Home a joint
 * @param  joint The joint to home
 * @return       True on success
 */
bool CommandChannel::home(string joint){
    BoardCommand command;
    memset(&command, 0, sizeof(command));
    int jointNum = 0;

    if (strcmp(joint.c_str(), "all") == 0){
        command.type = D_GOTO_HOME_ALL;
    } else if ((jointNum = indexLookup(joint)) == -1){
        return false;
    } else {
        command.type = D_GOTO_HOME;
        command.joint = jointNum;
    }

    int r = ach_put(&huboBoardCommandChannel, &command, sizeof(command));

    if (ACH_OK != r) {
        cerr << "Error! Command home failed with state " << r << endl;
        return false;
    }
    return true;
}

/**
 * Reset a joint
 * @param  joint The joint to reset
 * @return       True on successs
 */
bool CommandChannel::reset(string &joint){
    BoardCommand command;
    memset(&command, 0, sizeof(command));
    int jointNum = 0;

    if ((jointNum = indexLookup(joint)) == -1){
        return false;
    } else {
        command.type = D_ZERO_ENCODER;
        command.joint = jointNum;
    }

    int r = ach_put(&huboBoardCommandChannel, &command, sizeof(command));

    if (ACH_OK != r) {
        cerr << "Error! Command reset failed with state " << r << endl;
        return false;
    }
    return true;
}

/**
 * Initialize all sensors
 * @return True on success
 */
bool CommandChannel::initializeSensors(){
    BoardCommand command;
    memset(&command, 0, sizeof(command));

    command.type = D_NULL_SENSORS_ALL;

    int r = ach_put(&huboBoardCommandChannel, &command, sizeof(command));

    if (ACH_OK != r) {
        cerr << "Error! Command initializeSensors failed with state " << r << endl;
        return false;
    }
    return true;

}
