/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { HostContext } from '../lib/host_context'

import * as style from './terms_of_service.style'

export function TermsOfService () {
  const { getString } = React.useContext(HostContext)
  return (
    <style.terms>
      <span dangerouslySetInnerHTML={{ __html: getString('termsOfService') }} />
    </style.terms>
  )
}
