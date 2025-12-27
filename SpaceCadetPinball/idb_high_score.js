// idb_high_score.js

const DB_NAME = 'PinballHighScores';
const DB_VERSION = 1;
const STORE_NAME = 'highScores';
const HIGH_SCORES_KEY = 'highScores';

var PinballIDB = {
    $JSEvents: {
        //
        // A queue of JS async events that are waiting to be resolved.
        //
        queue: [],

        //
        // Add a promise to the queue.
        //
        // Returns the queue handle.
        //
        add: function(promise) {
            var event = {
                promise: promise,
                isResolved: false
            };

            // Find a null spot in the queue to reuse.
            var handle = -1;
            for (var i = 0; i < JSEvents.queue.length; i++) {
                if (JSEvents.queue[i] === null) {
                    handle = i;
                    break;
                }
            }

            if (handle === -1) {
                handle = JSEvents.queue.length;
                JSEvents.queue.push(event);
            } else {
                JSEvents.queue[handle] = event;
            }

            promise.then(function(result) {
                event.isResolved = true;
                event.resolveValue = result;
            }).catch(function(error) {
                event.isResolved = true;
                event.isRejected = true;
                event.rejectValue = error;
            });

            // The queue handle is the index of the event in the queue.
            return handle;
        },
    },

    openDB: function() {
        return new Promise((resolve, reject) => {
            if (!('indexedDB' in window)) {
                reject('IndexedDB not supported');
                return;
            }
            const request = indexedDB.open(DB_NAME, DB_VERSION);

            request.onupgradeneeded = (event) => {
            const db = event.target.result;
            if (!db.objectStoreNames.contains(STORE_NAME)) {
                db.createObjectStore(STORE_NAME, { keyPath: 'id' });
            }
            };

            request.onsuccess = (event) => {
            resolve(event.target.result);
            };

            request.onerror = (event) => {
            console.error('Error opening database:', event.target.errorCode);
            reject('Error opening database');
            };
        });
    },

    // C++ method bindings, prefix with _
    _ReadHighScores: function() {
        return JSEvents.add(new Promise(async (resolve, reject) => {
            try {
            const db = await PinballIDB.openDB();
            const transaction = db.transaction([STORE_NAME], 'readonly');
            const objectStore = transaction.objectStore(STORE_NAME);
            const request = objectStore.get(HIGH_SCORES_KEY);

            request.onsuccess = (event) => {
                // The result is a table of {Name: string, Score: int}. C++ expects a JSON string.
                if (event.target.result) {
                    var scores = JSON.stringify(event.target.result.scores);
                    var buffer = _malloc(scores.length + 1);
                    stringToUTF8(scores, buffer, scores.length + 1);
                    resolve(buffer);
                } else {
                    resolve(0); // NULL pointer
                }
            };

            request.onerror = (event) => {
                console.error('Error reading high scores:', event.target.errorCode);
                reject('Error reading high scores');
            };
            } catch (error) {
            reject(error.message || 'An unknown error occurred during read');
            }
        }));
    },

    _WriteHighScores: function(scores) {
        scores = UTF8ToString(scores);
        return JSEvents.add(new Promise(async (resolve, reject) => {
            try {
            const scores_obj = JSON.parse(scores);
            const db = await PinballIDB.openDB();
            const transaction = db.transaction([STORE_NAME], 'readwrite');
            const objectStore = transaction.objectStore(STORE_NAME);

            const request = objectStore.put({id: HIGH_SCORES_KEY, scores: scores_obj});

            transaction.oncomplete = () => {
                resolve(1);
            };

            transaction.onerror = (event) => {
                console.error('Error writing high scores:', event.target.errorCode);
                reject('Error writing high scores');
            };

            } catch (error) {
            reject(error.message || 'An unknown error occurred during write');
            }
        }));
    },

    _ClearHighScores: function() {
        return JSEvents.add(new Promise(async (resolve, reject) => {
            try {
                const db = await PinballIDB.openDB();
                const transaction = db.transaction([STORE_NAME], 'readwrite');
                const objectStore = transaction.objectStore(STORE_NAME);
                const request = objectStore.delete(HIGH_SCORES_KEY);

                transaction.oncomplete = () => {
                    resolve(1);
                };

                transaction.onerror = (event) => {
                    console.error('Error clearing high scores:', event.target.errorCode);
                    reject('Error clearing high scores');
                };
            }
            catch (error) {
                reject(error.message || 'An unknown error occurred during clear');
            }
        }));
    },

    // JSEvents C++ bindings, prefix with _
    _JSEvents_IsQueueEmpty: function() {
        return JSEvents.queue.length == 0;
    },
    _JSEvents_IsResolved: function(handle) {
        return JSEvents.queue[handle] != null && JSEvents.queue[handle].isResolved;
    },
    _JSEvents_IsRejected: function(handle) {
        return JSEvents.queue[handle] != null && JSEvents.queue[handle].isRejected;
    },
    _JSEvents_GetResolveValue: function(handle) {
        return JSEvents.queue[handle].resolveValue;
    },
    _JSEvents_GetRejectValue: function(handle) {
        var result = JSEvents.queue[handle].rejectValue;
        if (typeof result === 'string')
        {
            var buffer = _malloc(result.length + 1);
            stringToUTF8(result, buffer, result.length + 1);
            return buffer;
        }
        return result;
    },
    _JSEvents_FreeEvent: function(handle) {
        JSEvents.queue[handle] = null;
    },
};

autoAddDeps(PinballIDB, '$JSEvents');
mergeInto(LibraryManager.library, PinballIDB);
