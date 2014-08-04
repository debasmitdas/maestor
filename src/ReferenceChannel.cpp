/**
 * The hubo-ach Reference Ach channel. This is where we set joint references for hubo-ach 
 * to command the joint to. 
 */

#include "../include/ReferenceChannel.h"

/**
 * list of all the joints 
 */
const char *ReferenceChannel::urdf_joint_names[] = {
        "WST", "NKY", "NK1", "NK2",
        "LSP", "LSR", "LSY", "LEB", "LWY", "LWR", "LWP",
        "RSP", "RSR", "RSY", "REB", "RWY", "RWR", "RWP",
        "UNUSED1",
        "LHY", "LHR", "LHP", "LKN", "LAP", "LAR",
        "UNUSED2",
        "RHY", "RHR", "RHP", "RKN", "RAP", "RAR",
        "RF1", "RF2", "RF3", "RF4", "RF5",
        "LF1", "LF2", "LF3", "LF4", "LF5",
        "unknown1", "unknown2", "unknown3", "unknown4", "unknown5", "unknown6", "unknown7", "unknown8"};

/**
 * Constructor that initializes the ach channel
 */
ReferenceChannel::ReferenceChannel() {
    errored = false;

    int r = ach_open(&huboReferenceChannel, HUBO_CHAN_REF_NAME, NULL);
    if (ACH_OK != r && ACH_MISSED_FRAME != r && ACH_STALE_FRAMES != r){
        cout << "Error! Reference Channel failed with state " << r << endl;
        errored = true;
    }

}

/**
 * Destructor
 */
ReferenceChannel::~ReferenceChannel() {}

/**
 * Look up the index of a joint from the name of the joint
 * @param  joint Name of the joint
 * @return       The index of the joint
 */
int ReferenceChannel::indexLookup(string &joint) {
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
 * Load the reference channel
 */
void ReferenceChannel::load(){
    if (errored) return;
    Reference temp;
    memset( &temp, 0, sizeof(temp));
    size_t fs;

    int r = ach_get(&huboReferenceChannel, &temp, sizeof(temp), &fs, NULL, ACH_O_LAST);

    if(ACH_OK != r && ACH_MISSED_FRAME != r && ACH_STALE_FRAMES != r) {
        cout << "Error! Reference Channel failed with state " << r << endl;
        errored = true;
    } else if (ACH_STALE_FRAMES != r){
        if (sizeof(temp) != fs) {
            cout << "Error! File size inconsistent with state struct! fs = " << fs << " sizeof currentReference: " << sizeof(temp) << endl;
            errored = true;
            return;
        }
    } else
        return;
    currentReference = temp;
}

/**
 * Set the reference of a joint to a position in radians
 * 
 * @param joint The joint to set 
 * @param rad   The position in radians
 * @param mode  The mode of joint operation
 */
void ReferenceChannel::setReference(string &joint, double rad, Mode mode){
	if (errored) return;
	int index = indexLookup(joint);
	if (index != -1){
		currentReference.ref[index] = rad;
        currentReference.mode[index] = 1;
    }
}

/**
 * Update the reference channel
 */
void ReferenceChannel::update(){
    if (errored) return;
    ach_put(&huboReferenceChannel, &currentReference, sizeof(currentReference));
}
