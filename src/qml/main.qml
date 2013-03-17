/*
 A News Blur client for Ubuntu Phone
 Copyright (C) 2013  Jimi Smith <smithj002@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

import QtQuick 2.0
import QtWebKit 3.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import NewsBlur 0.1

MainView {
    id: applicationView
    applicationName: "News Blur"
    width: units.gu(40)
    height: units.gu(75)

    property string qtwebkitdpr: "1.0"

    function showLoading() {
        loadingIndicator.running = true
    }

    function hideLoading() {
        loadingIndicator.running = false
    }

    function showAuthView(authUrl) {
        showLoading();
        PopupUtils.open(loginDialog);
    }

    function showSubList() {
        hideLoading();
    }

    function showError(message) {
        errorHolder.showMessage(message);
    }

    NewsBlurApi {
        id: api
    }

    ActivityIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        running: true
    }

    Component {
        id: loginDialog
        Dialog {
            id: dialog
            title: "Login to NewsBlur"

            Label {
                text: "Username"
            }
            TextField {
                id: usernameInput
            }

            Label {
                text: "Password"
            }
            TextField {
                id: passwordInput
                echoMode: TextInput.Password
            }

            NewsBlurResponse {
                id: response
                onResponseReceived: {
                    if (ok) {
                        if (result.authenticated) {
                            PopupUtils.close(dialog);
                        } else {
                            sendButton.visible = true;
                            showError("Your username and password do not match");
                        }
                    } else {
                        sendButton.visible = true;
                        showError("There was an error contacting the server");
                    }
                }
            }

            Button {
                id: sendButton
                text: "Login"
                onClicked: {
                    sendButton.visible = false;
                    api.login(usernameInput.text, passwordInput.text, response);
                }
            }
        }
    }

    Button {
        id: loginButton
        anchors.centerIn: parent
        text: "Login"
        width: units.gu(16)
        onClicked: PopupUtils.open(loginDialog, loginButton)
        visible: false
    }

    Component.onCompleted: {
        loginButton.visible = true;
    }

    Item {
        id: errorHolder
        property string errorText: "An error occured"

        function showMessage(message) {
            errorText = message;
            PopupUtils.open(errorDialog, applicationView);
        }

        Component {
            id: errorDialog
            Dialog {
                id: error
                title: "Error"
                text: errorHolder.errorText

                Button {
                    text: "Okay"
                    onClicked: {
                        PopupUtils.close(error);
                    }
                }
            }
        }
    }

}
