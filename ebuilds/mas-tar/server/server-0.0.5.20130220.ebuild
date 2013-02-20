# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

AUTOTOOLS_AUTORECONF=1

DESCRIPTION="mastar tools library"
HOMEPAGE="http://zocromas.mastarink.net/zocromas/tools.html"
SRC_URI="http://zocromas.mastarink.net/zocromas/${P}.tar.bz2"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~amd64"
IUSE=""

DEPEND="dev-libs/glib:2 mas-tar/zoctypes mas-tar/mas_wrap
mas-tar/mas_init
mas-tar/mas_listener
mas-tar/mas_log
mas-tar/mas_modules
mas-tar/mas_msg
mas-tar/mas_thtools
mas-tar/mas_tools
mas-tar/mas_channel
mas-tar/mas_transaction
mas-tar/zocprotos
mas-tar/zocmds1
mas-tar/zocmds2
sys-apps/util-linux
"
RDEPEND="${DEPEND}"

src_configure() {
    econf  --enable-silent-rules --enable-tracemem --enable-debug
}

src_install() {
    emake DESTDIR="${D}" install

#    dodoc FAQ NEWS README
#    dohtml EXTENDING.html ctags.html
}
