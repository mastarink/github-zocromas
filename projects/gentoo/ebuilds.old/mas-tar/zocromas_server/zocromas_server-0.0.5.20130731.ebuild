# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

AUTOTOOLS_AUTORECONF=1

DESCRIPTION="mastar zoc server"
HOMEPAGE="http://zocromas.mastarink.net/zocromas/${PN}.html"
SRC_URI="http://zocromas.mastarink.net/zocromas/${P}.tar.bz2"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~amd64"
IUSE=""

DEPEND="dev-libs/glib:2
	sys-apps/util-linux
	mas-tar/zocromas_zoctypes
	mas-tar/zocromas_mas_wrap
	mas-tar/zocromas_mas_init
	mas-tar/zocromas_mas_listener
	mas-tar/zocromas_mas_log
	mas-tar/zocromas_mas_modules
	mas-tar/zocromas_mas_msg
	mas-tar/zocromas_mas_thtools
	mas-tar/zocromas_mas_tools
	mas-tar/zocromas_mas_channel
	mas-tar/zocromas_mas_transaction
"
RDEPEND="${DEPEND}"

src_configure() {
    econf  --enable-silent-rules
}

src_install() {
    emake DESTDIR="${D}" install

	newinitd "${FILESDIR}"/zocromas_server.initd zocromas_server   || die
	newconfd "${FILESDIR}"/zocromas_server.confd zocromas_server   || die
	newenvd  "${FILESDIR}"/zocromas_server.envd  50zocromas_server || die

#    dodoc FAQ NEWS README
#    dohtml EXTENDING.html ctags.html
}
